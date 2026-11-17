#include "Misc/AutomationTest.h"
#if WITH_DEV_AUTOMATION_TESTS
#include "ExperimentAdapter/BootstrapMarker.h"
#include "ExperimentAdapter/ExperimentWorldDirector.h"
#include "ExperimentAdapter/ExperimentSubsystem.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Tests/AutomationCommon.h"
#include "UObject/UObjectGlobals.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBootstrapRescueTeardownChecks, "UEgame.WorldScenario.B01.RescueTeardown",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::NonNullRHI | EAutomationTestFlags::ProductFilter)
bool FBootstrapRescueTeardownChecks::RunTest(const FString&)
{
    UWorld* World = nullptr;
    for (const auto& Context : GEngine->GetWorldContexts())
        if (Context.WorldType == EWorldType::Game) World = Context.World();
    if (!TestNotNull(TEXT("live game world"), World)) return false;
    auto* Adapter = World->GetGameInstance()->GetSubsystem<UExperimentSubsystem>();
    AExperimentWorldDirector* Director = nullptr;
    for (TActorIterator<AExperimentWorldDirector> It(World); It; ++It) Director = *It;
    if (!TestNotNull(TEXT("adapter"), Adapter) || !TestNotNull(TEXT("director"), Director)) return false;
    const auto Before = Adapter->View();
    if (!TestTrue(TEXT("session exists"), Before.IsSet())) return false;
    if (!TestTrue(TEXT("teardown starts before observation"),
        Before->phase == experiment::ExperimentPhase::Rescue && Director->HasObservableAftermath())) return false;
    const auto OldRun = Before->runId;
    const FTransform Transform = Director->GetActorTransform();
    const TWeakObjectPtr<AExperimentWorldDirector> OldDirector(Director);
    TArray<TWeakObjectPtr<ABootstrapMarker>> OldMarkers;
    for (TActorIterator<ABootstrapMarker> It(World); It; ++It) OldMarkers.Add(*It);
    TestEqual(TEXT("seven initial markers"), OldMarkers.Num(), 7);
    if (!TestTrue(TEXT("destroy active Rescue director"), World->DestroyActor(Director))) return false;
    TestFalse(TEXT("destroyed Rescue binding rejected"), Adapter->IsCurrentBinding(Director, OldRun));
    TestFalse(TEXT("late ready before GC rejected"), Adapter->NotifyBootstrapReady(Director, OldRun));
    TestFalse(TEXT("late observation before GC rejected"), Adapter->ObserveBootstrapAftermath(Director, OldRun));
    TestTrue(TEXT("teardown does not invent a terminal result"), Adapter->View()->phase == experiment::ExperimentPhase::Rescue);

    const TWeakObjectPtr<UWorld> WeakWorld(World);
    const TWeakObjectPtr<UExperimentSubsystem> WeakAdapter(Adapter);
    // Leave the destruction stack before a full collection. Never retain a raw actor across GC.
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.1f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, WeakWorld, WeakAdapter, OldDirector, OldMarkers, OldRun, Transform]()
    {
        CollectGarbage(RF_NoFlags, true);
        if (!TestTrue(TEXT("world and subsystem survive collection"), WeakWorld.IsValid() && WeakAdapter.IsValid())) return true;
        TestNull(TEXT("old director actually collected"), OldDirector.GetEvenIfUnreachable());
        for (const auto& Marker : OldMarkers)
            TestNull(TEXT("old owned marker actually collected"), Marker.GetEvenIfUnreachable());
        auto* CurrentAdapter = WeakAdapter.Get();
        // Model a delayed callback resolving its weak sender after GC, never a dangling UObject pointer.
        TestFalse(TEXT("collected sender cannot ready"), CurrentAdapter->NotifyBootstrapReady(OldDirector.Get(), OldRun));
        TestFalse(TEXT("collected sender cannot observe"), CurrentAdapter->ObserveBootstrapAftermath(OldDirector.Get(), OldRun));
        auto* Replacement = WeakWorld->SpawnActor<AExperimentWorldDirector>(AExperimentWorldDirector::StaticClass(), Transform);
        if (!TestNotNull(TEXT("replacement spawned after GC"), Replacement)) return true;
        const auto After = CurrentAdapter->View();
        if (!TestTrue(TEXT("replacement projection exists"), After.IsSet())) return true;
        TestTrue(TEXT("new round is ready Rescue"), After->runId > OldRun && After->worldStartedAt.has_value() &&
            After->phase == experiment::ExperimentPhase::Rescue);
        TestFalse(TEXT("old run cannot ready new actor"), CurrentAdapter->NotifyBootstrapReady(Replacement, OldRun));
        TestFalse(TEXT("old run cannot observe new actor"), CurrentAdapter->ObserveBootstrapAftermath(Replacement, OldRun));
        TestFalse(TEXT("collected sender cannot ready replacement"), CurrentAdapter->NotifyBootstrapReady(OldDirector.Get(), OldRun));
        TestFalse(TEXT("collected sender cannot observe replacement"), CurrentAdapter->ObserveBootstrapAftermath(OldDirector.Get(), OldRun));
        TestTrue(TEXT("late callbacks preserve new Rescue phase"), CurrentAdapter->View()->phase == experiment::ExperimentPhase::Rescue);
        CurrentAdapter->UnbindWorld(OldDirector.Get());
        TestTrue(TEXT("late collected sender unbind preserves new round"), CurrentAdapter->IsCurrentBinding(Replacement, After->runId));
        int32 Directors = 0, Markers = 0;
        for (TActorIterator<AExperimentWorldDirector> It(WeakWorld.Get()); It; ++It) ++Directors;
        for (TActorIterator<ABootstrapMarker> It(WeakWorld.Get()); It; ++It) ++Markers;
        TestEqual(TEXT("one replacement director"), Directors, 1);
        TestEqual(TEXT("only seven replacement markers"), Markers, 7);
        TestTrue(TEXT("replacement can complete its own observation"), CurrentAdapter->ObserveBootstrapAftermath(Replacement, After->runId));
        TestTrue(TEXT("only replacement round ends"), CurrentAdapter->View()->runId == After->runId &&
            CurrentAdapter->View()->phase == experiment::ExperimentPhase::Ended);
        return true;
    }));
    return true;
}
#endif
