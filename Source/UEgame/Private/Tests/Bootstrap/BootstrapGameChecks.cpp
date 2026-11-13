#include "Misc/AutomationTest.h"
#if WITH_DEV_AUTOMATION_TESTS
#include "ExperimentAdapter/BootstrapGameMode.h"
#include "ExperimentAdapter/ExperimentSubsystem.h"
#include "ExperimentAdapter/ExperimentWorldDirector.h"
#include "Components/TextRenderComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerController.h"
#include "Tests/AutomationCommon.h"
#include "UnrealClient.h"
#include "Engine/Font.h"
#include "Materials/MaterialInterface.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBootstrapGameChecks, "UEgame.WorldScenario.B01.Bootstrap",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::NonNullRHI | EAutomationTestFlags::ProductFilter)
bool FBootstrapGameChecks::RunTest(const FString&)
{
    UWorld* World = nullptr;
    for (const FWorldContext& Context : GEngine->GetWorldContexts())
        if (Context.WorldType == EWorldType::Game) World = Context.World();
    if (!TestNotNull(TEXT("live game world"), World)) return false;
    TestTrue(TEXT("bootstrap map loaded"), World->GetMapName() == TEXT("B01Bootstrap"));
    TestTrue(TEXT("bootstrap game mode"), IsValid(World->GetAuthGameMode<ABootstrapGameMode>()));
    int32 Directors = 0;
    for (TActorIterator<AExperimentWorldDirector> It(World); It; ++It)
    {
        ++Directors;
        TestTrue(TEXT("world assembly registered"), It->IsBootstrapAssembled());
        const auto* Text = It->FindComponentByClass<UTextRenderComponent>();
        if (TestNotNull(TEXT("feedback component"), Text))
        {
            TestEqual(TEXT("accepted projection reaches feedback"), Text->Text.ToString(), FString(TEXT("B01: bootstrap ready")));
            TestNotNull(TEXT("cooked font"), Text->Font.Get());
            TestNotNull(TEXT("cooked text material"), Text->GetMaterial(0));
            AddInfo(FString::Printf(TEXT("Text location=%s visible=%d hidden=%d"),
                *Text->GetComponentLocation().ToString(), Text->IsVisible(), Text->bHiddenInGame));
        }
    }
    TestEqual(TEXT("one director"), Directors, 1);
    const auto* Adapter = World->GetGameInstance()->GetSubsystem<UExperimentSubsystem>();
    if (!TestNotNull(TEXT("session adapter"), Adapter)) return false;
    const auto View = Adapter->View();
    if (TestTrue(TEXT("session projection exists"), View.IsSet()))
    {
        TestTrue(TEXT("real BeginPlay receipt starts world"), View->worldStartedAt.has_value());
        TestTrue(TEXT("core enters rescue"), View->phase == experiment::ExperimentPhase::Rescue);
    }
    const APlayerController* Controller = World->GetFirstPlayerController();
    if (TestNotNull(TEXT("local controller"), Controller))
    {
        TestTrue(TEXT("camera assigned"), IsValid(Cast<ACameraActor>(Controller->GetViewTarget())));
        FVector Location;
        FRotator Rotation;
        Controller->GetPlayerViewPoint(Location, Rotation);
        AddInfo(FString::Printf(TEXT("Actual camera location=%s rotation=%s"), *Location.ToString(), *Rotation.ToString()));
        TestTrue(TEXT("camera faces feedback"), FVector::DotProduct(Rotation.Vector(), (FVector(0, 0, 100) - Location).GetSafeNormal()) > 0.99f);
    }
    // Rendered text and end-to-end latency still require separate observation.
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]()
    {
        FScreenshotRequest::RequestScreenshot(TEXT("B01Bootstrap"), false, true);
        return true;
    }));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    return true;
}
#endif
