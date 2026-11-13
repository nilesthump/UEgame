#include "ExperimentSubsystem.h"
#include "ExperimentWorldDirector.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "HAL/PlatformTime.h"

experiment::RunId UExperimentSubsystem::BindWorld(AExperimentWorldDirector* Director)
{
    check(IsInGameThread());
    if (!IsValid(Director) || Director->IsActorBeingDestroyed() || !Director->GetWorld() || !Director->GetWorld()->IsGameWorld() ||
        Director->GetWorld()->GetGameInstance() != GetGameInstance() ||
        Director->GetWorld() != GetGameInstance()->GetWorld() || BoundDirector.IsValid())
        return 0;
    if (Session) Session->InvalidatePendingEffects();
    Session = MakeUnique<experiment::ExperimentSession>(NextRunId++);
    BoundDirector = Director;
    NextSequence = 1; NextBatch = 1;
    return Session->View().runId;
}
bool UExperimentSubsystem::IsCurrentBinding(const AExperimentWorldDirector* Director, experiment::RunId Run) const
{
    return IsValid(Director) && !Director->IsActorBeingDestroyed() && Director->GetWorld() &&
        BoundDirector.Get() == Director && Session &&
        Run != 0 && Session->View().runId == Run &&
        Director->GetWorld() == GetGameInstance()->GetWorld() &&
        Director->GetWorld()->GetGameInstance() == GetGameInstance();
}
void UExperimentSubsystem::UnbindWorld(AExperimentWorldDirector* Director)
{
    check(IsInGameThread());
    if (BoundDirector.Get() != Director) return;
    if (Session) Session->InvalidatePendingEffects();
    BoundDirector.Reset();
}
bool UExperimentSubsystem::NotifyBootstrapReady(AExperimentWorldDirector* Director, experiment::RunId Run)
{
    check(IsInGameThread());
    if (!IsCurrentBinding(Director, Run) || !Director->IsBootstrapAssembled()) return false;
    return Session->NotifyWorldReady({Run,
        {Director->GetWorld()->GetTimeSeconds(), FPlatformTime::Seconds()}});
}
TOptional<experiment::ExperimentViewProjection> UExperimentSubsystem::View() const
{
    if (!Session) return {};
    return Session->View();
}
void UExperimentSubsystem::Deinitialize()
{
    if (Session) Session->InvalidatePendingEffects();
    BoundDirector.Reset();
    Session.Reset();
    Super::Deinitialize();
}

experiment::FactContext UExperimentSubsystem::NextContext(experiment::RunId Run)
{
    return {Run, NextSequence++, {GetGameInstance()->GetWorld()->GetTimeSeconds(), FPlatformTime::Seconds()}};
}
bool UExperimentSubsystem::ObserveBootstrapAftermath(AExperimentWorldDirector* Director, experiment::RunId Run)
{
    check(IsInGameThread());
    if (!IsCurrentBinding(Director, Run) || !Director->HasObservableAftermath()) return false;
    const auto Effects = Session->NotifyAftermathObserved({NextContext(Run)});
    if (!Effects.accepted) return false;
    // Receipts retain occurrence context and drain only after the entire effect batch.
    struct SnapshotReceipt { experiment::SettlementCompletedFact Fact; experiment::EncounterSnapshot Snapshot; };
    TArray<SnapshotReceipt> Receipts;
    for (const auto& Effect : Effects.commands)
    {
        if (!IsCurrentBinding(Director, Run) || !Session->TryConsumeEffect(Effect)) return false;
        if (Effect.kind != experiment::EffectKind::RequestStableSnapshot) return false;
        const auto Batch = NextBatch++;
        const auto Snapshot = Director->SampleBootstrapEncounter(Run, Batch);
        if (!Snapshot.valid) return false;
        Receipts.Add({{NextContext(Run), Batch, true}, Snapshot});
    }
    for (const auto& Receipt : Receipts)
    {
        if (!IsCurrentBinding(Director, Run)) return false;
        const auto Settled = Session->NotifyBatchSettled(Receipt.Fact, Receipt.Snapshot);
        if (!Settled.accepted) return false;
        for (const auto& Effect : Settled.commands)
        {
            if (!IsCurrentBinding(Director, Run) || !Session->TryConsumeEffect(Effect) ||
                !Director->ExecuteBootstrapEffect(Effect)) return false;
        }
    }
    return true;
}
