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
