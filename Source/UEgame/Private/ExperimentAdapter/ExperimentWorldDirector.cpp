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
#include "BootstrapTextPreview.h"
#include "Engine/GameViewportClient.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

AExperimentWorldDirector::AExperimentWorldDirector()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;
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
    bRescueFixture = FParse::Param(FCommandLine::Get(), TEXT("B01RescueGraybox"));
    SetActorTickEnabled(bRescueFixture);
    if (bRescueFixture || FParse::Param(FCommandLine::Get(), TEXT("B01AftermathGraybox")))
    {
        for (int32 Index = 0; Index < 7; ++Index)
        {
            auto* Marker = GetWorld()->SpawnActor<ABootstrapMarker>(
                GetActorLocation() + FVector(0, 0, -60.f - Index * 28.f), FRotator::ZeroRotator);
            Markers.Add(Marker);
            if (Marker) Marker->InitializeMarker(Index == 0 ? TEXT("Player") :
                Index == 1 ? TEXT("Child") : FString::Printf(TEXT("Threat %d"), Index - 1), bRescueFixture || Index != 1);
        }
        if (auto* Controller = GetWorld()->GetFirstPlayerController())
        {
            EnableInput(Controller);
            InputComponent->BindKey(EKeys::E, IE_Pressed, this, bRescueFixture
                ? &AExperimentWorldDirector::AdvanceBootstrapRescue : &AExperimentWorldDirector::ObserveBootstrapAftermath);
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
                    const TCHAR* Prompt = TEXT("B01: bootstrap ready");
                    if (bRescueFixture) Prompt = TEXT("GRAYBOX: E = spot threats");
                    else if (!Markers.IsEmpty()) Prompt = TEXT("B01 GRAYBOX: E = observe aftermath");
                    StatusText->SetText(FText::FromString(Prompt));
                    UE_LOG(LogTemp, Display, TEXT("B01 bootstrap world ready: Run=%llu"),
                        static_cast<unsigned long long>(BindingRun));
                }
            }
        }
    }
}
void AExperimentWorldDirector::EndPlay(const EEndPlayReason::Type Reason)
{
    if (TextPreview.IsValid() && GetWorld()->GetGameViewport())
        GetWorld()->GetGameViewport()->RemoveViewportWidgetContent(TextPreview.ToSharedRef());
    TextPreview.Reset();
    RemoveTownDialogue();
    RemoveTransitionCover();
    if (Adapter.IsValid()) Adapter->UnbindWorld(this);
    for (const auto& Marker : Markers) if (IsValid(Marker)) Marker->Destroy();
    Markers.Reset();
    for (const auto& Marker : TownMarkers) if (IsValid(Marker)) Marker->Destroy();
    TownMarkers.Reset();
    BindingRun = 0;
    Adapter.Reset();
    Super::EndPlay(Reason);
}

bool AExperimentWorldDirector::HasObservableAftermath() const
{
    if (bRescueFixture || bGrayboxStopped || Markers.Num() != 7) return false;
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
ABootstrapMarker* AExperimentWorldDirector::NextRescueThreat() const
{
    if (!bRescueFixture || bGrayboxStopped || Markers.Num() != 7) return nullptr;
    ABootstrapMarker* Next = nullptr;
    for (int32 Index = 0; Index < Markers.Num(); ++Index)
    {
        auto* Marker = Markers[Index].Get();
        if (!IsValid(Marker) || !Marker->IsReady() || Marker->GetWorld() != GetWorld()) return nullptr;
        if (Index >= 2 && !Next && Marker->IsAlive()) Next = Marker;
    }
    return Next;
}
void AExperimentWorldDirector::AdvanceBootstrapRescue()
{
    if (Adapter.IsValid() && Adapter->AdvanceBootstrapRescue(this, BindingRun) && !bGrayboxStopped)
        StatusText->SetText(FText::FromString(TEXT("GRAYBOX: E = sword strike")));
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
        if (View->crowdContentDelivered && View->teacherRecallDelivered)
        {
            DisableInput(GetWorld()->GetFirstPlayerController());
            SetActorTickEnabled(false);
            StatusText->SetText(FText::FromString(TEXT("B01 GRAYBOX: revisit complete")));
            return true;
        }
        StatusText->SetText(FText::FromString(TEXT("B01 GRAYBOX: ended - child dead")));
        ShowBootstrapTextPreview();
        return true;
    }
    if (Effect.kind == experiment::EffectKind::BeginTimeSkip && bGrayboxStopped)
    {
        return BeginBootstrapSkip(Effect.requestId);
    }
    if (Effect.kind == experiment::EffectKind::RestoreControl && TransitionStep == ETransitionStep::FadeIn)
    {
        TransitionStep = ETransitionStep::Restoring;
        return true;
    }
    return false;
}

void AExperimentWorldDirector::ShowBootstrapTextPreview()
{
    // Separate opt-in: draft content is a display probe, not production narration.
    if (!FParse::Param(FCommandLine::Get(), TEXT("B01DraftTextPreview")) ||
        TextPreview.IsValid() || !GetWorld()->GetGameViewport()) return;
    FString Csv;
    FBootstrapTextEntry Entry;
    if (!FFileHelper::LoadFileToString(Csv, *(FPaths::ProjectDir() / TEXT("spec/contracts/narrative/TEXT.csv"))) ||
        !LoadBootstrapTextPreview(Csv, Entry))
    {
        UE_LOG(LogTemp, Error, TEXT("B01 draft preview: missing or invalid NAR-0004 source"));
        return;
    }
    const FString Display = FString::Printf(TEXT("B01 DRAFT PREVIEW | %s | %s\n%s: %s"),
        *Entry.Id, *Entry.Status, *Entry.Speaker, *Entry.Text);
    TextPreview = SNew(SBox).VAlign(VAlign_Bottom).HAlign(HAlign_Center).Padding(FMargin(24.f, 24.f, 24.f, 48.f))
        [SNew(SBorder).Padding(16.f).BorderBackgroundColor(FLinearColor::Black)
            [SNew(STextBlock).Text(FText::FromString(Display))
                .Font(FCoreStyle::GetDefaultFontStyle(TEXT("Regular"), 24))
                .ColorAndOpacity(FLinearColor::White).WrapTextAt(1050.f)]];
    TextPreview->SetVisibility(EVisibility::HitTestInvisible);
    GetWorld()->GetGameViewport()->AddViewportWidgetContent(TextPreview.ToSharedRef(), 10);
    UE_LOG(LogTemp, Display, TEXT("B01 draft preview attached: %s (%s)"), *Entry.Id, *Entry.Status);
}
