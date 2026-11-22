#include "ExperimentWorldDirector.h"
#include "Algo/Find.h"
#include "BootstrapMarker.h"
#include "ExperimentSubsystem.h"
#include "Components/TextRenderComponent.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Console.h"
#include "Engine/World.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/PlayerController.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Styling/CoreStyle.h"
#include "UnrealClient.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SWindow.h"

namespace
{
// Sample the graybox viewport every painted frame. A pause or missed paint
// restarts the interval even when its endpoints would otherwise qualify.
constexpr double MaxPaintGap = 0.25;
constexpr double SpeakingRange = 90.0;
}

class SBootstrapDialogueText final : public STextBlock
{
public:
    TFunction<bool()> IsEligible;
    mutable double FirstPaint = 0.0;
    mutable double LastPaint = 0.0;
    mutable uint64 LastFrame = 0;

    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& Geometry, const FSlateRect& CullRect,
        FSlateWindowElementList& Elements, int32 Layer, const FWidgetStyle& Style, bool bParentEnabled) const override
    {
        const auto Result = STextBlock::OnPaint(Args, Geometry, CullRect, Elements, Layer, Style, bParentEnabled);
        const auto Bounds = Geometry.GetRenderBoundingRect();
        const auto Size = Geometry.GetLocalSize();
        const auto Desired = GetDesiredSize();
        if (!IsEligible || !IsEligible() || Bounds.Left < CullRect.Left - 0.5f || Bounds.Right > CullRect.Right + 0.5f ||
            Bounds.Top < CullRect.Top - 0.5f || Bounds.Bottom > CullRect.Bottom + 0.5f ||
            Size.X + 0.5f < Desired.X || Size.Y + 0.5f < Desired.Y || Size.GetMin() <= 0.f)
        {
            FirstPaint = LastPaint = 0.0;
            return Result;
        }
        const double Now = FPlatformTime::Seconds();
        if (FirstPaint == 0.0 || GFrameCounter > LastFrame + 1 || Now - LastPaint > MaxPaintGap)
            FirstPaint = Now;
        LastPaint = Now;
        LastFrame = GFrameCounter;
        return Result;
    }

    bool HasContinuousPaint(double RequiredSeconds) const
    {
        return FirstPaint > 0.0 && GFrameCounter <= LastFrame + 1 &&
            FPlatformTime::Seconds() - LastPaint <= MaxPaintGap && LastPaint - FirstPaint >= RequiredSeconds;
    }
};

bool AExperimentWorldDirector::LoadTownDialogue()
{
    if (bTownDialogueChecked) return TownRoute != nullptr;
    const auto Outcome = Adapter->Session->RescueOutcome();
    if (!Outcome || !Outcome->succeeded || !Outcome->method) return false;
    bTownDialogueChecked = true;

    using experiment::RescueMethod;
    using experiment::RevisitContentId;
    static constexpr FTownRoute Routes[] = {
        {RescueMethod::Sword, RevisitContentId::CrowdSword, RevisitContentId::TeacherSword,
            TEXT("sword"), TEXT("Sword teacher"), TEXT("Sword students")},
        {RescueMethod::Magic, RevisitContentId::CrowdMagic, RevisitContentId::TeacherMagic,
            TEXT("magic"), TEXT("Magic teacher"), TEXT("Magic students")},
        {RescueMethod::Mixed, RevisitContentId::CrowdMixed, RevisitContentId::TeacherMixed,
            TEXT("mixed"), TEXT("Sword and magic teacher"), TEXT("Sword and magic students")}
    };
    const auto* Route = Algo::FindBy(Routes, *Outcome->method, &FTownRoute::Method);
    if (!Route) return false;
    FString Csv;
    FBootstrapTextEntry Crowd, Teacher;
    auto LoadLine = [&](RevisitContentId Id, const TCHAR* SpeakerRole, FBootstrapTextEntry& Line)
    {
        return LoadBootstrapTextEntry(Csv, *FString::Printf(TEXT("NAR-%04d"), static_cast<int32>(Id)),
            *FString::Printf(TEXT("revisit.%s.%s"), SpeakerRole, Route->SceneSuffix), Line);
    };
    if (!FFileHelper::LoadFileToString(Csv, *(FPaths::ProjectDir() / TEXT("spec/contracts/narrative/TEXT.csv"))) ||
        !LoadLine(Route->Crowd, TEXT("crowd"), Crowd) || !LoadLine(Route->Teacher, TEXT("teacher"), Teacher))
    {
        UE_LOG(LogTemp, Error, TEXT("B01 town: missing or invalid %s crowd/teacher text; town readiness held"), Route->SceneSuffix);
        return false;
    }
    TownRoute = Route;
    CrowdLine = MoveTemp(Crowd);
    TeacherLine = MoveTemp(Teacher);
    return true;
}

bool AExperimentWorldDirector::IsTownSpeakerEligible(bool bTeacher) const
{
    auto* World = GetWorld();
    if (!World || World->IsPaused() || !Adapter.IsValid() || !Adapter->IsCurrentBinding(this, BindingRun) ||
        Adapter->View()->phase != experiment::ExperimentPhase::Revisit || !IsTownReady()) return false;
    auto* Controller = World->GetFirstPlayerController();
    auto* Client = World->GetGameViewport();
    if (!Controller || !Controller->IsLocalController() || !Client || !Client->Viewport ||
        !Client->Viewport->HasFocus() || !FSlateApplication::IsInitialized() ||
        (Client->ViewportConsole && Client->ViewportConsole->ConsoleActive()) ||
        FSlateApplication::Get().AnyMenusVisible() || FSlateApplication::Get().GetActiveModalWindow().IsValid() ||
        !Client->GetWindow().IsValid() || !Client->GetWindow()->IsActive()) return false;

    auto* Player = TownMarkers[0].Get();
    auto* Speaker = TownMarkers[bTeacher ? 1 : 2].Get();
    const FVector From = Player->GetActorLocation(), To = Speaker->GetActorLocation();
    if (FVector::DistSquared(From, To) > FMath::Square(SpeakingRange)) return false;
    if (!bTeacher) return true;
    FVector2D Screen;
    const auto Size = Client->Viewport->GetSizeXY();
    if (!Controller->ProjectWorldLocationToScreen(To, Screen) || Screen.X < 0 || Screen.Y < 0 ||
        Screen.X >= Size.X || Screen.Y >= Size.Y) return false;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(B01TownMeeting), false, Player);
    Params.AddIgnoredActor(Speaker);
    return !World->LineTraceTestByChannel(From, To, ECC_Visibility, Params);
}

FString AExperimentWorldDirector::GetActiveTownTextId() const
{
    return DialogueText.IsValid() ? (bTeacherSpeaking ? TeacherLine.Id : CrowdLine.Id) : FString();
}

void AExperimentWorldDirector::ShowTownDialogue(bool bTeacher)
{
    bTeacherSpeaking = bTeacher;
    const auto& Line = bTeacher ? TeacherLine : CrowdLine;
    // This fixture has no bound voice asset. Allocate a fresh text-fallback
    // generation, with no elapsed credit from voice preparation.
    ++DialogueGeneration;
    const auto Display = FString::Printf(TEXT("%s | %s | TextFallback\n%s: %s"),
        *Line.Id, *Line.Status, *Line.Speaker, *Line.Text);
    DialoguePanel = SNew(SBox).HAlign(HAlign_Center).VAlign(VAlign_Bottom).Padding(16.f)
        [SNew(SBorder).BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush")).BorderBackgroundColor(FLinearColor::Black).Padding(12.f)
            [SAssignNew(DialogueText, SBootstrapDialogueText).Text(FText::FromString(Display))
                .Font(FCoreStyle::GetDefaultFontStyle(TEXT("Regular"), 22)).ColorAndOpacity(FLinearColor::White)
                .WrapTextAt(560.f)]];
    DialogueText->IsEligible = [Director = TWeakObjectPtr<AExperimentWorldDirector>(this), bTeacher]()
    {
        // Slate can continue painting while a paused world no longer ticks.
        return Director.IsValid() && Director->IsTownSpeakerEligible(bTeacher);
    };
    DialogueText->ForceVolatile(true);
    DialoguePanel->SetVisibility(EVisibility::HitTestInvisible);
    GetWorld()->GetGameViewport()->AddViewportWidgetContent(DialoguePanel.ToSharedRef(), 30);
    UE_LOG(LogTemp, Display, TEXT("B01 town text fallback (no bound voice): %s run=%llu generation=%llu"),
        *Line.Id, BindingRun, DialogueGeneration);
}

void AExperimentWorldDirector::RemoveTownDialogue()
{
    if (DialoguePanel && GetWorld() && GetWorld()->GetGameViewport())
        GetWorld()->GetGameViewport()->RemoveViewportWidgetContent(DialoguePanel.ToSharedRef());
    DialoguePanel.Reset();
    DialogueText.Reset();
}

void AExperimentWorldDirector::UpdateTownDialogue()
{
    const auto View = Adapter->View();
    if (View->phase != experiment::ExperimentPhase::Revisit) return;
    if (DialogueText.IsValid())
    {
        if (!IsTownSpeakerEligible(bTeacherSpeaking))
        {
            RemoveTownDialogue();
            return;
        }
        const auto& Line = bTeacherSpeaking ? TeacherLine : CrowdLine;
        // Graybox reading allowance: six Chinese characters per second,
        // plus one second to orient to the speaker.
        const double RequiredSeconds = 1.0 + Line.Text.Len() / 6.0;
        if (!DialogueText->HasContinuousPaint(RequiredSeconds)) return;
        const auto Id = bTeacherSpeaking ? TownRoute->Teacher : TownRoute->Crowd;
        RemoveTownDialogue();
        if (Adapter->ReportTownContent(this, BindingRun, Id, DialogueGeneration))
            UE_LOG(LogTemp, Display, TEXT("B01 town text delivered: %s run=%llu generation=%llu"), *Line.Id, BindingRun, DialogueGeneration);
        return;
    }
    if (!View->teacherRecallDelivered && IsTownSpeakerEligible(true)) ShowTownDialogue(true);
    else if (!View->crowdContentDelivered && IsTownSpeakerEligible(false)) ShowTownDialogue(false);
}
