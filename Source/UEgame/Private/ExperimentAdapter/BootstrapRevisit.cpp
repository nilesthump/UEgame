#include "ExperimentWorldDirector.h"
#include "BootstrapMarker.h"
#include "ExperimentSubsystem.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBorder.h"

#include "Widgets/Text/STextBlock.h"

class SBootstrapTransitionCover final : public SBorder
{
public:
    mutable bool bPaintedOpaque = false;
    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& Geometry, const FSlateRect& CullRect,
        FSlateWindowElementList& Elements, int32 Layer, const FWidgetStyle& Style, bool bParentEnabled) const override
    {
        const int32 Result = SBorder::OnPaint(Args, Geometry, CullRect, Elements, Layer, Style, bParentEnabled);
        if (GetRenderOpacity() >= 1.f && Geometry.GetLocalSize().GetMin() > 0.f) bPaintedOpaque = true;
        return Result;
    }
};

void AExperimentWorldDirector::AdvanceOldEra(double Until)
{
    // The sword fixture contains no burning or cut material. Advance its two scheduled
    // weather boundaries using the same cursor during live play and fast-forward.
    OldEraSeconds = FMath::Max(OldEraSeconds, Until);
    if (ConsumedRainEvents == 0 && OldEraSeconds >= 180.0)
    {
        ++ConsumedRainEvents;
        UE_LOG(LogTemp, Display, TEXT("B01 old era: rain started at 180, run=%llu"), BindingRun);
    }
    if (ConsumedRainEvents == 1 && OldEraSeconds >= 300.0)
    {
        ++ConsumedRainEvents;
        UE_LOG(LogTemp, Display, TEXT("B01 old era: rain ended at 300, run=%llu"), BindingRun);
    }
}

bool AExperimentWorldDirector::BeginBootstrapSkip(uint64 Token)
{
    auto* Viewport = GetWorld()->GetGameViewport();
    if (!bRescueFixture || !Viewport || TransitionCover.IsValid()) return false;
    const auto View = Adapter->View();
    AdvanceOldEra(GetWorld()->GetTimeSeconds() - View->worldStartedAt->engineWorldSeconds);
    SkipToken = Token;
    StatusText->SetText(FText::FromString(TEXT("TimeSkip: awaiting environment")));
    TransitionCover = SNew(SBootstrapTransitionCover)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .BorderBackgroundColor(FLinearColor::Black)
        .HAlign(HAlign_Center).VAlign(VAlign_Center)
        [SNew(STextBlock).Text(FText::FromString(TEXT("30 years later")))
            .Font(FCoreStyle::GetDefaultFontStyle(TEXT("Regular"), 28))];
    TransitionCover->SetVisibility(EVisibility::HitTestInvisible);
    TransitionCover->SetRenderOpacity(0.f);
    Viewport->AddViewportWidgetContent(TransitionCover.ToSharedRef(), 20);
    TransitionStep = ETransitionStep::FadeOut;
    return true;
}

void AExperimentWorldDirector::RemoveTransitionCover()
{
    if (TransitionCover.IsValid() && GetWorld() && GetWorld()->GetGameViewport())
        GetWorld()->GetGameViewport()->RemoveViewportWidgetContent(TransitionCover.ToSharedRef());
    TransitionCover.Reset();
}

bool AExperimentWorldDirector::IsTownCovered() const
{
    return TransitionCover.IsValid() && CoverOpacity >= 1.f && TransitionCover->bPaintedOpaque;
}

bool AExperimentWorldDirector::RevealReadyTown(float DeltaSeconds)
{
    if (!IsTownReady())
    {
        CoverOpacity = 1.f;
        TransitionCover->SetRenderOpacity(CoverOpacity);
        return false;
    }
    if (CoverOpacity >= 1.f && !IsTownCovered()) return false;
    CoverOpacity = FMath::Max(0.f, CoverOpacity - DeltaSeconds);
    TransitionCover->SetRenderOpacity(CoverOpacity);
    if (CoverOpacity < 1.f) TransitionCover->bPaintedOpaque = false;
    return CoverOpacity <= 0.f;
}

void AExperimentWorldDirector::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (!Adapter.IsValid() || !Adapter->IsCurrentBinding(this, BindingRun)) return;
    switch (TransitionStep)
    {
    case ETransitionStep::Idle:
    {
        const auto View = Adapter->View();
        if (View->phase == experiment::ExperimentPhase::Rescue)
            AdvanceOldEra(GetWorld()->GetTimeSeconds() - View->worldStartedAt->engineWorldSeconds);
        break;
    }
    case ETransitionStep::FadeOut:
        CoverOpacity = FMath::Min(1.f, CoverOpacity + DeltaSeconds);
        TransitionCover->SetRenderOpacity(CoverOpacity);
        if (CoverOpacity >= 1.f) TransitionStep = ETransitionStep::Covered;
        break;
    case ETransitionStep::Covered:
        if (!TransitionCover->bPaintedOpaque) break;
        AdvanceOldEra(300.0);
        if (Adapter->ReportSkipReady(this, BindingRun, SkipToken, experiment::SkipMilestone::EnvironmentSettled))
            TransitionStep = ETransitionStep::PrepareTown;
        break;
    case ETransitionStep::PrepareTown:
        if (PrepareTown() && Adapter->ReportSkipReady(this, BindingRun, SkipToken, experiment::SkipMilestone::TownPrepared))
            TransitionStep = ETransitionStep::FadeIn;
        break;
    case ETransitionStep::FadeIn:
        if (RevealReadyTown(DeltaSeconds))
        {
            Adapter->ReportSkipReady(this, BindingRun, SkipToken, experiment::SkipMilestone::TransitionFinished);
        }
        break;
    case ETransitionStep::Restoring:
        if (RevealReadyTown(DeltaSeconds) && RestoreTownInput() && Adapter->ReportControlRestored(this, BindingRun, SkipToken))
        {
            RemoveTransitionCover();
            TransitionStep = ETransitionStep::Complete;
        }
        break;
    case ETransitionStep::Complete:
        UpdateTownDialogue();
        break;
    default:
        break;
    }
}

bool AExperimentWorldDirector::PrepareTown()
{
    if (ConsumedRainEvents != 2 || Markers.Num() != 7) return false;
    for (const auto& Marker : Markers)
        if (!IsValid(Marker) || !Marker->IsReady() || Marker->GetWorld() != GetWorld()) return false;

    if (!LoadTownDialogue()) return false;
    // The teacher has a fresh town identity. Retire the rescue cast only once all
    // three replacement markers have registered successfully behind the cover.
    const FVector Origin = GetActorLocation();
    const FVector Offsets[] = {FVector(0, 0, -145), FVector(0, -140, -65), FVector(0, 140, -65)};
    const TCHAR* Labels[] = {TEXT("Town player"), TownRoute->TeacherLabel, TownRoute->StudentsLabel};
    const FName Roles[] = {TEXT("B01TownPlayer"), TEXT("B01Teacher"), TEXT("B01Crowd")};
    for (int32 Index = 0; Index < UE_ARRAY_COUNT(Offsets); ++Index)
    {
        auto* Marker = GetWorld()->SpawnActor<ABootstrapMarker>(Origin + Offsets[Index], FRotator::ZeroRotator);
        if (Marker) TownMarkers.Add(Marker);
        if (!Marker || !Marker->IsReady())
        {
            for (const auto& Spawned : TownMarkers) if (IsValid(Spawned)) Spawned->Destroy();
            TownMarkers.Reset();
            return false;
        }
        Marker->InitializeMarker(Labels[Index], true);
        Marker->Tags.Add(Roles[Index]);
    }
    for (const auto& Marker : Markers) Marker->Destroy();
    Markers.Reset();
    StatusText->SetText(FText::FromString(TEXT("Revisit +30y: town")));
    return true;
}

bool AExperimentWorldDirector::IsTownReady() const
{
    if (TownMarkers.Num() != 3) return false;
    for (const auto& Marker : TownMarkers)
        if (!IsValid(Marker) || !Marker->IsReady() || Marker->GetWorld() != GetWorld() || Marker->IsHidden()) return false;
    return true;
}

bool AExperimentWorldDirector::RestoreTownInput()
{
    auto* Controller = GetWorld()->GetFirstPlayerController();
    if (TransitionStep != ETransitionStep::Restoring || !Controller || !Controller->IsLocalController() || !InputComponent || !IsTownReady()) return false;
    InputComponent->ClearActionBindings();
    InputComponent->KeyBindings.Reset();
    InputComponent->BindKey(EKeys::A, IE_Pressed, this, &AExperimentWorldDirector::MoveTownLeft);
    InputComponent->BindKey(EKeys::D, IE_Pressed, this, &AExperimentWorldDirector::MoveTownRight);
    EnableInput(Controller);
    StatusText->SetText(FText::FromString(TEXT("Revisit +30y: A/D = move")));
    return true;
}

void AExperimentWorldDirector::MoveTownLeft() { MoveInTown(35.f); }
void AExperimentWorldDirector::MoveTownRight() { MoveInTown(-35.f); }
void AExperimentWorldDirector::MoveInTown(float Distance)
{
    if (!Adapter.IsValid() || !Adapter->IsCurrentBinding(this, BindingRun) ||
        Adapter->View()->phase != experiment::ExperimentPhase::Revisit || !IsTownReady()) return;
    auto Position = TownMarkers[0]->GetActorLocation();
    Position.Y = FMath::Clamp(Position.Y + Distance, GetActorLocation().Y - 210., GetActorLocation().Y + 210.);
    TownMarkers[0]->SetActorLocation(Position);
}
