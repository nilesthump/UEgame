#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "experiment/Session/ExperimentSession.h"
#include "ExperimentWorldDirector.generated.h"
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
protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type Reason) override;
private:
    UPROPERTY()
    TObjectPtr<UTextRenderComponent> StatusText;
    TWeakObjectPtr<UExperimentSubsystem> Adapter;
    experiment::RunId BindingRun = 0;
};
