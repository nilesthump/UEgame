#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "experiment/Session/ExperimentSession.h"
#include "ExperimentWorldDirector.generated.h"
class ABootstrapMarker;
class UTextRenderComponent;
class UExperimentSubsystem;

// B01 bootstrap only. No encounter health, navigation or media readiness is asserted.
UCLASS()
class AExperimentWorldDirector final : public AActor
{
    GENERATED_BODY()
public:
    AExperimentWorldDirector();
    bool IsBootstrapAssembled() const;
    bool HasObservableAftermath() const;
    experiment::EncounterSnapshot SampleBootstrapEncounter(experiment::RunId Run, experiment::BatchId Batch) const;

    void ObserveBootstrapAftermath();
protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type Reason) override;
private:
    friend class UExperimentSubsystem;
    bool ExecuteBootstrapEffect(const experiment::ExperimentEffect& Effect);
    UPROPERTY()
    TObjectPtr<UTextRenderComponent> StatusText;
    UPROPERTY() TArray<TObjectPtr<ABootstrapMarker>> Markers;
    bool bGrayboxStopped = false;
    TWeakObjectPtr<UExperimentSubsystem> Adapter;
    experiment::RunId BindingRun = 0;
};
