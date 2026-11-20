#include "Misc/AutomationTest.h"
#if WITH_DEV_AUTOMATION_TESTS
#include "ExperimentAdapter/BootstrapMarker.h"
#include "ExperimentAdapter/ExperimentSubsystem.h"
#include "ExperimentAdapter/ExperimentWorldDirector.h"
#include "Components/TextRenderComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "HAL/PlatformTime.h"
#include "InputKeyEventArgs.h"
#include "InputCoreTypes.h"
#include "Tests/AutomationCommon.h"
#include "UnrealClient.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBootstrapRescueSuccessChecks, "UEgame.WorldScenario.B01.RescueSuccess",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::NonNullRHI | EAutomationTestFlags::ProductFilter)

bool FBootstrapRescueSuccessChecks::RunTest(const FString&)
{
    UWorld* World = nullptr;
    for (const auto& Context : GEngine->GetWorldContexts())
        if (Context.WorldType == EWorldType::Game) World = Context.World();
    if (!TestNotNull(TEXT("live game world"), World)) return false;
    TActorIterator<AExperimentWorldDirector> Directors(World);
    if (!TestTrue(TEXT("director exists"), bool(Directors))) return false;
    const TWeakObjectPtr<AExperimentWorldDirector> Director(*Directors);
    const TWeakObjectPtr<UExperimentSubsystem> Adapter(World->GetGameInstance()->GetSubsystem<UExperimentSubsystem>());
    const TWeakObjectPtr<APlayerController> Controller(World->GetFirstPlayerController());
    if (!TestTrue(TEXT("bound adapter and input controller"), Adapter.IsValid() && Adapter->View().IsSet() && Controller.IsValid()))
        return false;
    const auto Run = Adapter->View()->runId;
    TestFalse(TEXT("old run cannot discover threats"), Adapter->AdvanceBootstrapRescue(Director.Get(), Run + 1));
    TActorIterator<ABootstrapMarker> Markers(World);
    if (!TestTrue(TEXT("rescue fixture has markers"), bool(Markers))) return false;
    Markers->GetRootComponent()->UnregisterComponent();
    TestFalse(TEXT("incomplete fixture rejects discovery"), Adapter->AdvanceBootstrapRescue(Director.Get(), Run));
    Markers->GetRootComponent()->RegisterComponent();
    TestFalse(TEXT("rejected requests leave discovery unset"), Adapter->View()->threatDiscoveredAt.has_value());

    // One discovery input followed by five independent attacks. Inspect the world between presses.
    for (int32 Step = 0; Step <= 5; ++Step)
    {
        ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Controller]()
        {
            if (!TestTrue(TEXT("controller still live"), Controller.IsValid())) return true;
            for (const auto Event : {IE_Pressed, IE_Released})
                Controller->InputKey(FInputKeyEventArgs(nullptr, FInputDeviceId::CreateFromInternalId(0),
                    EKeys::E, Event, FPlatformTime::Cycles64()));
            return true;
        }));
        ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.25f));
        ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Adapter, Director, Run, Step]()
        {
            if (!TestTrue(TEXT("fixture still bound"), Adapter.IsValid() && Director.IsValid() &&
                Adapter->IsCurrentBinding(Director.Get(), Run))) return true;
            int32 Total = 0;
            int32 Defeated = 0;
            for (TActorIterator<ABootstrapMarker> It(Director->GetWorld()); It; ++It)
            {
                ++Total;
                if (!It->IsAlive()) ++Defeated;
            }
            const auto Snapshot = Director->SampleBootstrapEncounter(Run, 1);
            TestTrue(TEXT("both survivors present in stable world"), Snapshot.valid && Snapshot.playerAlive && Snapshot.childAlive);
            TestEqual(TEXT("all seven markers present"), Total, 7);
            TestEqual(TEXT("one defeat per attack"), Defeated, Step);
            const auto View = Adapter->View().GetValue();
            TestTrue(TEXT("discovery delivered"), View.threatDiscoveredAt.has_value());
            TestEqual(TEXT("attack clock starts on first strike"), View.firstAttackAt.has_value(), Step > 0);
            TestEqual(TEXT("sword use counted after discovery"), View.swordUsed, Step > 0);
            TestFalse(TEXT("no magic actions"), View.magicUsed);
            TestTrue(TEXT("only final strike starts TimeSkip"), View.phase == (Step == 5 ?
                experiment::ExperimentPhase::TimeSkip : experiment::ExperimentPhase::Rescue));
            if (Step == 5)
            {
                TestFalse(TEXT("closed rescue rejects another strike"), Adapter->AdvanceBootstrapRescue(Director.Get(), Run));
                TestEqual(TEXT("BeginTimeSkip updates visible feedback"),
                    Director->FindComponentByClass<UTextRenderComponent>()->Text.ToString(), FString(TEXT("TimeSkip: awaiting environment")));
                FScreenshotRequest::RequestScreenshot(TEXT("B01RescueSuccess"), true, true);
            }
            return true;
        }));
    }
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f));
    return true;
}
#endif
