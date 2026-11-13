#include "Misc/AutomationTest.h"
#if WITH_DEV_AUTOMATION_TESTS
#include "ExperimentAdapter/BootstrapMarker.h"
#include "ExperimentAdapter/ExperimentWorldDirector.h"
#include "ExperimentAdapter/ExperimentSubsystem.h"
#include "Components/TextRenderComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "InputKeyEventArgs.h"
#include "InputCoreTypes.h"
#include "HAL/PlatformTime.h"
#include "Tests/AutomationCommon.h"
#include "UnrealClient.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBootstrapAftermathChecks, "UEgame.WorldScenario.B01.Aftermath",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::NonNullRHI | EAutomationTestFlags::ProductFilter)
bool FBootstrapAftermathChecks::RunTest(const FString&)
{
    UWorld* World = nullptr;
    for (const auto& Context : GEngine->GetWorldContexts())
        if (Context.WorldType == EWorldType::Game) World = Context.World();
    if (!TestNotNull(TEXT("live world"), World)) return false;
    auto* Adapter = World->GetGameInstance()->GetSubsystem<UExperimentSubsystem>();
    AExperimentWorldDirector* Director = nullptr;
    for (TActorIterator<AExperimentWorldDirector> It(World); It; ++It) Director = *It;
    if (!TestNotNull(TEXT("director"), Director) || !TestNotNull(TEXT("adapter"), Adapter)) return false;
    const auto View = Adapter->View();
    if (!TestTrue(TEXT("ready projection"), View.IsSet())) return false;
    const auto Run = View->runId;
    TestTrue(TEXT("waits in Rescue before observation"), View->phase == experiment::ExperimentPhase::Rescue);
    if (!TestTrue(TEXT("opt-in fixture required"), Director->HasObservableAftermath())) return false;
    TestFalse(TEXT("old run observation rejected"), Adapter->ObserveBootstrapAftermath(Director, Run + 1));
    ABootstrapMarker* Marker = *TActorIterator<ABootstrapMarker>(World);
    Marker->GetRootComponent()->UnregisterComponent();
    TestFalse(TEXT("missing registered participant invalidates snapshot"), Director->SampleBootstrapEncounter(Run, 1).valid);
    TestFalse(TEXT("incomplete world cannot submit observation"), Adapter->ObserveBootstrapAftermath(Director, Run));
    TestTrue(TEXT("invalid world cannot end session"), Adapter->View()->phase == experiment::ExperimentPhase::Rescue);
    Marker->GetRootComponent()->RegisterComponent();
    APlayerController* Controller = World->GetFirstPlayerController();
    if (!TestNotNull(TEXT("input controller"), Controller)) return false;
    Controller->InputKey(FInputKeyEventArgs(nullptr, FInputDeviceId::CreateFromInternalId(0), EKeys::E, IE_Pressed, FPlatformTime::Cycles64()));
    Controller->InputKey(FInputKeyEventArgs(nullptr, FInputDeviceId::CreateFromInternalId(0), EKeys::E, IE_Released, FPlatformTime::Cycles64()));
    const TWeakObjectPtr<AExperimentWorldDirector> WeakDirector(Director);
    const TWeakObjectPtr<UExperimentSubsystem> WeakAdapter(Adapter);
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.5f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, WeakDirector, WeakAdapter, Run]()
    {
        if (!TestTrue(TEXT("bindings survive input"), WeakDirector.IsValid() && WeakAdapter.IsValid())) return true;
        TestTrue(TEXT("input through snapshot receipt ends core"), WeakAdapter->View()->phase == experiment::ExperimentPhase::Ended);
        TestFalse(TEXT("stop effect closes observation"), WeakDirector->HasObservableAftermath());
        TestFalse(TEXT("duplicate observation cannot replay effects"), WeakAdapter->ObserveBootstrapAftermath(WeakDirector.Get(), Run));
        TestEqual(TEXT("typed stop effect updates visible feedback"),
            WeakDirector->FindComponentByClass<UTextRenderComponent>()->Text.ToString(), FString(TEXT("B01 GRAYBOX: ended - child dead")));
        FScreenshotRequest::RequestScreenshot(TEXT("B01Aftermath"), false, true);
        return true;
    }));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f));
    return true;
}
#endif
