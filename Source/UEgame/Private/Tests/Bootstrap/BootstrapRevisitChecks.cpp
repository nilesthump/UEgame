#include "Misc/AutomationTest.h"
#if WITH_DEV_AUTOMATION_TESTS
#include "ExperimentAdapter/BootstrapMarker.h"
#include "ExperimentAdapter/ExperimentSubsystem.h"
#include "ExperimentAdapter/ExperimentWorldDirector.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "HAL/PlatformTime.h"
#include "InputCoreTypes.h"
#include "InputKeyEventArgs.h"
#include "Tests/AutomationCommon.h"
#include "UnrealClient.h"

namespace
{
ABootstrapMarker* FindTownMarker(UWorld* World, FName Tag)
{
    for (TActorIterator<ABootstrapMarker> It(World); It; ++It)
        if (It->ActorHasTag(Tag)) return *It;
    return nullptr;
}
void PressKey(APlayerController& Controller, const FKey& Key)
{
    for (const auto Event : {IE_Pressed, IE_Released})
        Controller.InputKey(FInputKeyEventArgs(nullptr, FInputDeviceId::CreateFromInternalId(0),
            Key, Event, FPlatformTime::Cycles64()));
}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBootstrapRevisitChecks, "UEgame.WorldScenario.B01.Revisit",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::NonNullRHI | EAutomationTestFlags::ProductFilter)

bool FBootstrapRevisitChecks::RunTest(const FString&)
{
    UWorld* World = nullptr;
    for (const auto& Context : GEngine->GetWorldContexts())
        if (Context.WorldType == EWorldType::Game) World = Context.World();
    if (!TestNotNull(TEXT("game world"), World)) return false;

    TActorIterator<AExperimentWorldDirector> Directors(World);
    if (!TestTrue(TEXT("world director"), bool(Directors))) return false;
    const TWeakObjectPtr<AExperimentWorldDirector> Director(*Directors);
    const TWeakObjectPtr<APlayerController> Controller(World->GetFirstPlayerController());
    const TWeakObjectPtr<UExperimentSubsystem> Adapter(World->GetGameInstance()->GetSubsystem<UExperimentSubsystem>());
    if (!TestTrue(TEXT("live rescue fixture"), Controller.IsValid() && Adapter.IsValid() &&
        Adapter->View().IsSet() && Adapter->View()->phase == experiment::ExperimentPhase::Rescue)) return false;
    const auto Run = Adapter->View()->runId;
    TArray<TWeakObjectPtr<ABootstrapMarker>> Encounter;
    for (TActorIterator<ABootstrapMarker> It(World); It; ++It) Encounter.Add(*It);
    if (!TestEqual(TEXT("original rescue participants"), Encounter.Num(), 7)) return false;

    for (int32 Press = 0; Press < 6; ++Press)
    {
        ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Controller]()
        {
            if (TestTrue(TEXT("rescue controller available"), Controller.IsValid())) PressKey(*Controller.Get(), EKeys::E);
            return true;
        }));
        ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.3f));
    }
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Adapter]()
    {
        TestTrue(TEXT("six inputs start the time skip"), Adapter.IsValid() && Adapter->View().IsSet() &&
            Adapter->View()->phase == experiment::ExperimentPhase::TimeSkip);
        return true;
    }));
    const double Deadline = FPlatformTime::Seconds() + 30.0;
    // Make the assembled town temporarily unavailable while the cover is fading in.
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Director, Adapter, Deadline]()
    {
        if (!Director.IsValid() || !Adapter.IsValid()) { AddError(TEXT("lost transition binding")); return true; }
        if (auto* Teacher = FindTownMarker(Director->GetWorld(), TEXT("B01Teacher")))
        {
            TestTrue(TEXT("layout is prepared before control returns"), Adapter->View()->phase == experiment::ExperimentPhase::TimeSkip);
            Teacher->GetRootComponent()->UnregisterComponent();
            return true;
        }
        if (FPlatformTime::Seconds() < Deadline) return false;
        AddError(TEXT("town layout never appeared"));
        return true;
    }));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Director, Adapter]()
    {
        if (!Director.IsValid() || !Adapter.IsValid()) { AddError(TEXT("lost pending town")); return true; }
        TestTrue(TEXT("incomplete town remains covered"), Director->IsTownCovered());
        TestTrue(TEXT("unavailable town holds control receipt"), Adapter->View()->phase == experiment::ExperimentPhase::TimeSkip &&
            !Adapter->View()->revisitStartedAt.has_value());
        if (auto* Teacher = FindTownMarker(Director->GetWorld(), TEXT("B01Teacher")))
            Teacher->GetRootComponent()->RegisterComponent();
        return true;
    }));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Adapter, Director, Run, Deadline]()
    {
        if (!TestTrue(TEXT("current run survives the transition"), Adapter.IsValid() && Director.IsValid() &&
            Adapter->IsCurrentBinding(Director.Get(), Run) && Adapter->View().IsSet())) return true;
        if (Adapter->View()->phase == experiment::ExperimentPhase::Revisit) return true;
        if (FPlatformTime::Seconds() < Deadline) return false;
        AddError(TEXT("Timed out waiting for player control in the revisit town"));
        return true;
    }));
    const TSharedRef<FVector2D> BeforeMove = MakeShared<FVector2D>(FVector2D::ZeroVector);
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Adapter, Director, Controller, Run, BeforeMove, Encounter]()
    {
        if (!TestTrue(TEXT("revisit binding and controller"), Director.IsValid() && Controller.IsValid() &&
            Adapter.IsValid() && Adapter->IsCurrentBinding(Director.Get(), Run))) return true;
        const auto View = Adapter->View();
        if (!TestTrue(TEXT("accepted control receipt sets revisit clock"), View.IsSet() &&
            View->phase == experiment::ExperimentPhase::Revisit && View->revisitStartedAt.has_value())) return true;
        TestEqual(TEXT("both old-era rain events consumed"), Director->GetConsumedRainEvents(), 2);
        TestTrue(TEXT("rain stop at old-era second 300 was reached"), Director->GetOldEraSeconds() >= 300.0);
        auto* Town = Director->GetWorld();
        auto* Player = FindTownMarker(Town, TEXT("B01TownPlayer"));
        if (!TestNotNull(TEXT("town player"), Player)) return true;
        TestNotNull(TEXT("teacher is present"), FindTownMarker(Town, TEXT("B01Teacher")));
        TestNotNull(TEXT("students are present"), FindTownMarker(Town, TEXT("B01Crowd")));
        TestFalse(TEXT("rescue is closed in the town"), Adapter->AdvanceBootstrapRescue(Director.Get(), Run));
        for (const auto& Original : Encounter)
            TestTrue(TEXT("rescue participant retired from view"), !Original.IsValid() || Original->IsHidden());
        TestTrue(TEXT("player projects onto town view"), Controller->ProjectWorldLocationToScreen(Player->GetActorLocation(), *BeforeMove));
        PressKey(*Controller.Get(), EKeys::D);
        return true;
    }));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.25f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Director, Controller, BeforeMove]()
    {
        if (!TestTrue(TEXT("director remains available"), Director.IsValid())) return true;
        const auto* Player = FindTownMarker(Director->GetWorld(), TEXT("B01TownPlayer"));
        if (!TestNotNull(TEXT("town player after input"), Player)) return true;
        FVector2D AfterMove = FVector2D::ZeroVector;
        if (!TestTrue(TEXT("player still in the view"), Controller.IsValid() &&
            Controller->ProjectWorldLocationToScreen(Player->GetActorLocation(), AfterMove))) return true;
        TestTrue(TEXT("D moves town player toward the right of the screen"), AfterMove.X > BeforeMove->X);
        FScreenshotRequest::RequestScreenshot(TEXT("B01Revisit"), true, true);
        return true;
    }));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Controller]()
    {
        if (Controller.IsValid()) PressKey(*Controller.Get(), EKeys::A);
        return true;
    }));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.25f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Director, Controller, BeforeMove]()
    {
        if (!TestTrue(TEXT("town and controller survive return movement"), Director.IsValid() && Controller.IsValid())) return true;
        const auto* Player = FindTownMarker(Director->GetWorld(), TEXT("B01TownPlayer"));
        FVector2D Returned = FVector2D::ZeroVector;
        if (TestTrue(TEXT("player projects after A"), Player && Controller->ProjectWorldLocationToScreen(Player->GetActorLocation(), Returned)))
            TestTrue(TEXT("A returns town player to original screen position"), Returned.Equals(*BeforeMove, 1.0));
        return true;
    }));
    return true;
}
#endif
