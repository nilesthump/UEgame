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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBootstrapLifecycleChecks, "UEgame.WorldScenario.B01.Lifecycle",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::NonNullRHI | EAutomationTestFlags::ProductFilter)
bool FBootstrapLifecycleChecks::RunTest(const FString&)
{
    UWorld* World = nullptr;
    for (const auto& Context : GEngine->GetWorldContexts())
        if (Context.WorldType == EWorldType::Game) World = Context.World();
    if (!TestNotNull(TEXT("live game world"), World)) return false;
    auto* Adapter = World->GetGameInstance()->GetSubsystem<UExperimentSubsystem>();
    AExperimentWorldDirector* Director = nullptr;
    for (TActorIterator<AExperimentWorldDirector> It(World); It; ++It) Director = *It;
    if (!TestNotNull(TEXT("adapter"), Adapter) || !TestNotNull(TEXT("director"), Director)) return false;
    if (!TestTrue(TEXT("requires aftermath fixture"), Director->HasObservableAftermath())) return false;

    // Exercise actual EndPlay/BeginPlay in a running game, retaining the same subsystem.
    for (int32 Cycle = 0; Cycle < 3; ++Cycle)
    {
        const auto Before = Adapter->View();
        if (!TestTrue(TEXT("bound session exists"), Before.IsSet())) return false;
        const auto OldRun = Before->runId;
        TestTrue(TEXT("observation accepted before teardown"), Adapter->ObserveBootstrapAftermath(Director, OldRun));
        TestTrue(TEXT("old round ends"), Adapter->View()->phase == experiment::ExperimentPhase::Ended);
        const FTransform Transform = Director->GetActorTransform();
        auto* OldDirector = Director;
        if (!TestTrue(TEXT("actual actor destruction succeeds"), World->DestroyActor(OldDirector))) return false;
        TestFalse(TEXT("destroyed director loses binding"), Adapter->IsCurrentBinding(OldDirector, OldRun));
        TestFalse(TEXT("late ready after destruction rejected"), Adapter->NotifyBootstrapReady(OldDirector, OldRun));
        TestFalse(TEXT("late observation after destruction rejected"), Adapter->ObserveBootstrapAftermath(OldDirector, OldRun));
        int32 RemainingMarkers = 0;
        for (TActorIterator<ABootstrapMarker> It(World); It; ++It) ++RemainingMarkers;
        TestEqual(TEXT("EndPlay removes all owned markers"), RemainingMarkers, 0);

        Director = World->SpawnActor<AExperimentWorldDirector>(AExperimentWorldDirector::StaticClass(), Transform);
        if (!TestNotNull(TEXT("replacement director spawned"), Director)) return false;
        const auto After = Adapter->View();
        if (!TestTrue(TEXT("replacement session exists"), After.IsSet())) return false;
        TestTrue(TEXT("new BeginPlay owns a fresh run"), After->runId > OldRun);
        TestTrue(TEXT("new BeginPlay submits readiness"), After->worldStartedAt.has_value());
        TestTrue(TEXT("terminal state does not leak"), After->phase == experiment::ExperimentPhase::Rescue);
        TestTrue(TEXT("replacement observation reopens"), Director->HasObservableAftermath());
        TestFalse(TEXT("old identity cannot ready replacement"), Adapter->NotifyBootstrapReady(Director, OldRun));
        TestFalse(TEXT("old identity cannot observe replacement"), Adapter->ObserveBootstrapAftermath(Director, OldRun));
        Adapter->UnbindWorld(OldDirector);
        TestTrue(TEXT("late old unbind preserves replacement"), Adapter->IsCurrentBinding(Director, After->runId));
        TestTrue(TEXT("rejected callbacks preserve fresh phase"), Adapter->View()->phase == experiment::ExperimentPhase::Rescue);
        int32 Directors = 0, Markers = 0;
        for (TActorIterator<AExperimentWorldDirector> It(World); It; ++It) ++Directors;
        for (TActorIterator<ABootstrapMarker> It(World); It; ++It) ++Markers;
        TestEqual(TEXT("one live director after replacement"), Directors, 1);
        TestEqual(TEXT("only seven fresh markers remain"), Markers, 7);
    }
    // Existing Aftermath covers the first input binding; this covers the rebuilt binding.
    auto* Controller = World->GetFirstPlayerController();
    if (!TestNotNull(TEXT("input controller"), Controller)) return false;
    const auto Run = Adapter->View()->runId;
    Controller->InputKey(FInputKeyEventArgs(nullptr, FInputDeviceId::CreateFromInternalId(0), EKeys::E, IE_Pressed, FPlatformTime::Cycles64()));
    Controller->InputKey(FInputKeyEventArgs(nullptr, FInputDeviceId::CreateFromInternalId(0), EKeys::E, IE_Released, FPlatformTime::Cycles64()));
    const TWeakObjectPtr<AExperimentWorldDirector> WeakDirector(Director);
    const TWeakObjectPtr<UExperimentSubsystem> WeakAdapter(Adapter);
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.5f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, WeakDirector, WeakAdapter, Run]()
    {
        if (!TestTrue(TEXT("rebuilt bindings survive input"), WeakDirector.IsValid() && WeakAdapter.IsValid())) return true;
        const auto View = WeakAdapter->View();
        if (!TestTrue(TEXT("projection survives input"), View.IsSet())) return true;
        TestTrue(TEXT("rebuilt input ends only current run"), View->runId == Run && View->phase == experiment::ExperimentPhase::Ended);
        TestFalse(TEXT("rebuilt input closes on stop"), WeakDirector->HasObservableAftermath());
        TestFalse(TEXT("duplicate current observation rejected"), WeakAdapter->ObserveBootstrapAftermath(WeakDirector.Get(), Run));
        TestEqual(TEXT("replacement feedback follows core"), WeakDirector->FindComponentByClass<UTextRenderComponent>()->Text.ToString(),
            FString(TEXT("B01 GRAYBOX: ended - child dead")));
        return true;
    }));
    return true;
}
#endif
