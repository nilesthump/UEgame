#include "ExperimentWorldDirector.h"
#include "ExperimentSubsystem.h"
#include "Components/TextRenderComponent.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "BootstrapMarker.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "InputCoreTypes.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"

AExperimentWorldDirector::AExperimentWorldDirector()
{
    PrimaryActorTick.bCanEverTick = false;
    StatusText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("BootstrapStatus"));
    SetRootComponent(StatusText);
    StatusText->SetText(FText::FromString(TEXT("B01: preparing bootstrap")));
    StatusText->SetWorldSize(40.f);
    StatusText->SetHorizontalAlignment(EHTA_Center);
}
bool AExperimentWorldDirector::IsBootstrapAssembled() const
{
    return !IsActorBeingDestroyed() && IsValid(StatusText) && StatusText->IsRegistered() &&
        StatusText->GetWorld() == GetWorld();
}
void AExperimentWorldDirector::BeginPlay()
{
    Super::BeginPlay();
    // Opt-in fixture: never claim these markers are implemented encounter gameplay.
    if (FParse::Param(FCommandLine::Get(), TEXT("B01AftermathGraybox")))
    {
        for (int32 Index = 0; Index < 7; ++Index)
        {
            auto* Marker = GetWorld()->SpawnActor<ABootstrapMarker>(
                GetActorLocation() + FVector(0, 0, -60.f - Index * 28.f), FRotator::ZeroRotator);
            Markers.Add(Marker);
            if (Marker) Marker->InitializeMarker(Index == 0 ? TEXT("Player") :
                Index == 1 ? TEXT("Child") : FString::Printf(TEXT("Threat %d"), Index - 1), Index != 1);
        }
        if (auto* Controller = GetWorld()->GetFirstPlayerController())
        {
            EnableInput(Controller);
            InputComponent->BindKey(EKeys::E, IE_Pressed, this, &AExperimentWorldDirector::ObserveBootstrapAftermath);
        }
    }
    if (UGameInstance* Instance = GetGameInstance())
    {
        Adapter = Instance->GetSubsystem<UExperimentSubsystem>();
        if (Adapter.IsValid())
        {
            BindingRun = Adapter->BindWorld(this);
            if (BindingRun != 0 && Adapter->NotifyBootstrapReady(this, BindingRun))
            {
                // Only the accepted Session projection drives this bootstrap feedback.
                const auto View = Adapter->View();
                if (View.IsSet() && View->phase == experiment::ExperimentPhase::Rescue)
                {
                    StatusText->SetText(FText::FromString(Markers.IsEmpty() ? TEXT("B01: bootstrap ready") : TEXT("B01 GRAYBOX: E = observe aftermath")));
                    UE_LOG(LogTemp, Display, TEXT("B01 bootstrap world ready: Run=%llu"),
                        static_cast<unsigned long long>(BindingRun));
                }
            }
        }
    }
}
void AExperimentWorldDirector::EndPlay(const EEndPlayReason::Type Reason)
{
    if (Adapter.IsValid()) Adapter->UnbindWorld(this);
    for (const auto& Marker : Markers) if (IsValid(Marker)) Marker->Destroy();
    Markers.Reset();
    BindingRun = 0;
    Adapter.Reset();
    Super::EndPlay(Reason);
}

bool AExperimentWorldDirector::HasObservableAftermath() const
{
    if (bGrayboxStopped || Markers.Num() != 7) return false;
    for (const auto& Marker : Markers)
        if (!IsValid(Marker) || !Marker->IsReady()) return false;
    return !Markers[1]->IsAlive();
}
experiment::EncounterSnapshot AExperimentWorldDirector::SampleBootstrapEncounter(
    experiment::RunId Run, experiment::BatchId Batch) const
{
    experiment::EncounterSnapshot Snapshot{Run, Batch};
    if (Markers.Num() != 7) return Snapshot;
    for (const auto& Marker : Markers)
        if (!IsValid(Marker) || !Marker->IsReady() || Marker->GetWorld() != GetWorld()) return Snapshot;
    Snapshot.valid = true;
    Snapshot.playerAlive = Markers[0]->IsAlive();
    Snapshot.childAlive = Markers[1]->IsAlive();
    Snapshot.allFiveThreatsDead = true;
    for (int32 Index = 2; Index < 7; ++Index) Snapshot.allFiveThreatsDead &= !Markers[Index]->IsAlive();
    return Snapshot;
}
void AExperimentWorldDirector::ObserveBootstrapAftermath()
{
    if (Adapter.IsValid() && HasObservableAftermath()) Adapter->ObserveBootstrapAftermath(this, BindingRun);
}
bool AExperimentWorldDirector::ExecuteBootstrapEffect(const experiment::ExperimentEffect& Effect)
{
    if (!Adapter.IsValid() || !Adapter->IsCurrentBinding(this, Effect.runId)) return false;
    if (Effect.kind == experiment::EffectKind::StopRescue)
    {
        bGrayboxStopped = true;
        DisableInput(GetWorld()->GetFirstPlayerController());
        return true;
    }
    if (Effect.kind == experiment::EffectKind::StopRun && bGrayboxStopped)
    {
        const auto View = Adapter->View();
        if (!View.IsSet() || View->phase != experiment::ExperimentPhase::Ended) return false;
        StatusText->SetText(FText::FromString(TEXT("B01 GRAYBOX: ended - child dead")));
        return true;
    }
    return false; // No fake TimeSkip/environment completion.
}
