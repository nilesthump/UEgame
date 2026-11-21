#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "experiment/Session/ExperimentSession.h"
#include "BootstrapTextPreview.h"
#include "ExperimentWorldDirector.generated.h"
class ABootstrapMarker;
class UTextRenderComponent;
class UExperimentSubsystem;
class SWidget;
class SBootstrapTransitionCover;
class SBootstrapDialogueText;

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
    int32 GetConsumedRainEvents() const { return ConsumedRainEvents; }
    double GetOldEraSeconds() const { return OldEraSeconds; }
    bool IsTownCovered() const;
    FString GetActiveTownTextId() const;
    virtual void Tick(float DeltaSeconds) override;
protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type Reason) override;
private:
    enum class ETransitionStep { Idle, FadeOut, Covered, PrepareTown, FadeIn, Restoring, Complete };
    ETransitionStep TransitionStep = ETransitionStep::Idle;
    uint64 SkipToken = 0;
    int32 ConsumedRainEvents = 0;
    double OldEraSeconds = 0;
    float CoverOpacity = 0;
    TSharedPtr<SBootstrapTransitionCover> TransitionCover;
    void AdvanceOldEra(double Until);
    bool BeginBootstrapSkip(uint64 Token);
    bool PrepareTown();
    bool IsTownReady() const;
    bool RevealReadyTown(float DeltaSeconds);
    UPROPERTY() TArray<TObjectPtr<ABootstrapMarker>> TownMarkers;
    bool RestoreTownInput();
    bool bTownDialogueChecked = false;
    FBootstrapTextEntry CrowdLine, TeacherLine;
    TSharedPtr<SWidget> DialoguePanel;
    TSharedPtr<SBootstrapDialogueText> DialogueText;
    uint64 DialogueGeneration = 0;
    bool bTeacherSpeaking = false;
    bool LoadTownDialogue();
    void UpdateTownDialogue();
    bool IsTownSpeakerEligible(bool bTeacher) const;
    void ShowTownDialogue(bool bTeacher);
    void RemoveTownDialogue();
    void MoveTownLeft();
    void MoveTownRight();
    void MoveInTown(float Distance);
    void RemoveTransitionCover();
    void AdvanceBootstrapRescue();
    ABootstrapMarker* NextRescueThreat() const;
    bool bRescueFixture = false;
    void ShowBootstrapTextPreview();
    TSharedPtr<SWidget> TextPreview;
    friend class UExperimentSubsystem;
    bool ExecuteBootstrapEffect(const experiment::ExperimentEffect& Effect);
    UPROPERTY()
    TObjectPtr<UTextRenderComponent> StatusText;
    UPROPERTY() TArray<TObjectPtr<ABootstrapMarker>> Markers;
    bool bGrayboxStopped = false;
    TWeakObjectPtr<UExperimentSubsystem> Adapter;
    experiment::RunId BindingRun = 0;
};
