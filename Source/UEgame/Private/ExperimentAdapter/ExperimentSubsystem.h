#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "experiment/Session/ExperimentSession.h"
#include "ExperimentSubsystem.generated.h"

class AExperimentWorldDirector;

// Session outlives world actors; binding identity prevents late world callbacks.
UCLASS()
class UExperimentSubsystem final : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    experiment::RunId BindWorld(AExperimentWorldDirector* Director);
    void UnbindWorld(AExperimentWorldDirector* Director);
    bool NotifyBootstrapReady(AExperimentWorldDirector* Director, experiment::RunId Run);
    bool IsCurrentBinding(const AExperimentWorldDirector* Director, experiment::RunId Run) const;
    TOptional<experiment::ExperimentViewProjection> View() const;
    bool ObserveBootstrapAftermath(AExperimentWorldDirector* Director, experiment::RunId Run);
private:
    experiment::FactContext NextContext(experiment::RunId Run);
    uint64 NextSequence = 1;
    experiment::BatchId NextBatch = 1;
public:
    virtual void Deinitialize() override;
private:
    TUniquePtr<experiment::ExperimentSession> Session;
    TWeakObjectPtr<AExperimentWorldDirector> BoundDirector;
    experiment::RunId NextRunId = 1;
};
