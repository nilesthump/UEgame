#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS
#include "ExperimentAdapter/ExperimentSubsystem.h"
#include "ExperimentAdapter/ExperimentWorldDirector.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Engine/Console.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/PlayerController.h"
#include "HAL/PlatformTime.h"
#include "InputCoreTypes.h"
#include "InputKeyEventArgs.h"
#include "Tests/AutomationCommon.h"
#include "UnrealClient.h"
#include "Widgets/SWindow.h"
#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
#endif

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBootstrapTownDialogueChecks, "UEgame.WorldScenario.B01.TownDialogue",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::NonNullRHI | EAutomationTestFlags::ProductFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBootstrapMagicTownDialogueChecks, "UEgame.WorldScenario.B01.MagicTownDialogue",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::NonNullRHI | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBootstrapMixedTownDialogueChecks, "UEgame.WorldScenario.B01.MixedTownDialogue",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::NonNullRHI | EAutomationTestFlags::ProductFilter)

namespace
{
bool RunTownDialogue(FAutomationTestBase& Test, int32 SwordStrikes, const TCHAR* CrowdId, const TCHAR* TeacherId,
    FString ScreenshotPrefix)
{
    UWorld* World = nullptr;
    for (const auto& Context : GEngine->GetWorldContexts())
        if (Context.WorldType == EWorldType::Game) World = Context.World();
    if (!Test.TestNotNull(TEXT("game world"), World)) return false;
    TActorIterator<AExperimentWorldDirector> Actors(World);
    if (!Test.TestTrue(TEXT("director exists"), bool(Actors))) return false;
    const TWeakObjectPtr<AExperimentWorldDirector> Director(*Actors);
    const TWeakObjectPtr<APlayerController> Controller(World->GetFirstPlayerController());
    const TWeakObjectPtr<UExperimentSubsystem> Adapter(World->GetGameInstance()->GetSubsystem<UExperimentSubsystem>());
    if (!Test.TestTrue(TEXT("rescue ready"), Controller.IsValid() && Adapter.IsValid() &&
        Adapter->View().IsSet() && Adapter->View()->phase == experiment::ExperimentPhase::Rescue)) return false;

#if PLATFORM_WINDOWS
    // External launchers can hide the HWND without updating Slate's cached visibility.
    auto* StartupViewport = World->GetGameViewport();
    const auto GameWindow = StartupViewport ? StartupViewport->GetWindow() : nullptr;
    const auto NativeWindow = GameWindow ? GameWindow->GetNativeWindow() : nullptr;
    const HWND Handle = NativeWindow ? static_cast<HWND>(NativeWindow->GetOSWindowHandle()) : nullptr;
    unsigned long WindowProcessId = 0;
    if (Handle && ::GetWindowThreadProcessId(Handle, &WindowProcessId) &&
        WindowProcessId == ::GetCurrentProcessId() && !::IsWindowVisible(Handle) && !::IsIconic(Handle))
    {
        Test.AddInfo(TEXT("Restoring hidden game viewport at town dialogue test startup"));
        GameWindow->Restore();
        if (!Test.TestTrue(TEXT("restored game viewport is visible at startup"), ::IsWindowVisible(Handle) != 0))
            return false;
        Test.AddInfo(TEXT("Game viewport restored and visibly shown at startup"));
    }
#endif

    auto Press = [&Test, Controller](FKey Key, int Count)
    {
        for (int Index = 0; Index < Count; ++Index)
        {
            ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([&Test, Controller, Key]()
            {
                if (!Test.TestTrue(TEXT("controller available"), Controller.IsValid())) return true;
                for (const auto Event : {IE_Pressed, IE_Released})
                    Controller->InputKey(FInputKeyEventArgs(nullptr, FInputDeviceId::CreateFromInternalId(0),
                        Key, Event, FPlatformTime::Cycles64()));
                return true;
            }));
            ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.12f));
        }
    };
    auto Await = [&Test, Director, Adapter](const TCHAR* Description, TFunction<bool()> Condition)
    {
        ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
            [&Test, Director, Adapter, Description, Condition, Deadline = 0.0]() mutable
            {
                if (!Director.IsValid() || !Adapter.IsValid() || !Adapter->View().IsSet())
                {
                    Test.AddError(TEXT("lost town binding"));
                    return true;
                }
                if (Condition()) return true;
                const double Now = FPlatformTime::Seconds();
                if (Deadline == 0.0) Deadline = Now + 20.0;
                if (Now < Deadline) return false;
                const UWorld* TownWorld = Director->GetWorld();
                UGameViewportClient* Client = TownWorld ? TownWorld->GetGameViewport() : nullptr;
                const auto Window = Client ? Client->GetWindow() : nullptr;
                const bool HasSlate = FSlateApplication::IsInitialized();
                Test.AddError(FString::Printf(
                    TEXT("%s (at timeout: phase=%d paused=%d viewportPresent=%d viewportFocused=%d windowPresent=%d ")
                    TEXT("windowActive=%d windowMinimized=%d slateReady=%d menu=%d modal=%d console=%d activeText='%s')"),
                    Description, static_cast<int32>(Adapter->View()->phase), TownWorld && TownWorld->IsPaused(), Client && Client->Viewport,
                    Client && Client->Viewport && Client->Viewport->HasFocus(), Window.IsValid(),
                    Window.IsValid() && Window->IsActive(), Window.IsValid() && Window->IsWindowMinimized(), HasSlate,
                    HasSlate && FSlateApplication::Get().AnyMenusVisible(),
                    HasSlate && FSlateApplication::Get().GetActiveModalWindow().IsValid(),
                    Client && Client->ViewportConsole && Client->ViewportConsole->ConsoleActive(),
                    *Director->GetActiveTownTextId()));
                return true;
            }));
    };
    Press(EKeys::E, 1); // Discover; this input does not count as a strike.
    Press(EKeys::E, SwordStrikes);
    Press(EKeys::F, 5 - SwordStrikes);
    Await(TEXT("control never restored"), [Adapter] { return Adapter->View()->phase == experiment::ExperimentPhase::Revisit; });
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([&Test, Adapter, SwordStrikes]()
    {
        Test.TestTrue(TEXT("rescue facts match the played actions"), Adapter.IsValid() && Adapter->View().IsSet() &&
            Adapter->View()->swordUsed == (SwordStrikes > 0) && Adapter->View()->magicUsed == (SwordStrikes < 5));
        return true;
    }));
    Press(EKeys::A, 4);
    Await(TEXT("crowd subtitle missing"), [Director, CrowdId] { return Director->GetActiveTownTextId() == CrowdId; });
    Press(EKeys::D, 4);
    Await(TEXT("leaving crowd must clear its subtitle"), [Director] { return Director->GetActiveTownTextId().IsEmpty(); });
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([&Test, Adapter]()
    {
        Test.TestTrue(TEXT("partial exposure earns neither receipt"), Adapter.IsValid() && Adapter->View().IsSet() &&
            !Adapter->View()->crowdContentDelivered && !Adapter->View()->teacherRecallDelivered);
        return true;
    }));
    Press(EKeys::A, 4);
    Await(TEXT("crowd subtitle never restarted"), [Director, CrowdId] { return Director->GetActiveTownTextId() == CrowdId; });
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.3f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([ScreenshotPrefix] { FScreenshotRequest::RequestScreenshot(ScreenshotPrefix + TEXT("Crowd"), true, true); return true; }));
    Await(TEXT("crowd receipt missing"), [Adapter] { return Adapter->View()->crowdContentDelivered; });
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([&Test, Adapter]()
    {
        Test.TestTrue(TEXT("crowd alone leaves revisit active"), Adapter.IsValid() && Adapter->View().IsSet() &&
            Adapter->View()->phase == experiment::ExperimentPhase::Revisit && !Adapter->View()->teacherRecallDelivered);
        return true;
    }));
    Press(EKeys::D, 8);
    Await(TEXT("teacher subtitle missing"), [Director, TeacherId] { return Director->GetActiveTownTextId() == TeacherId; });
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.3f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([ScreenshotPrefix] { FScreenshotRequest::RequestScreenshot(ScreenshotPrefix + TEXT("Teacher"), true, true); return true; }));
    Await(TEXT("two delivered clips must end the run"), [Adapter] {
        const auto View = Adapter->View();
        return View->crowdContentDelivered && View->teacherRecallDelivered && View->phase == experiment::ExperimentPhase::Ended;
    });
    return true;
}
}

bool FBootstrapTownDialogueChecks::RunTest(const FString&)
{
    return RunTownDialogue(*this, 5, TEXT("NAR-0001"), TEXT("NAR-0005"), TEXT("B01Town"));
}

bool FBootstrapMagicTownDialogueChecks::RunTest(const FString&)
{
    return RunTownDialogue(*this, 0, TEXT("NAR-0002"), TEXT("NAR-0006"), TEXT("B01MagicTown"));
}

bool FBootstrapMixedTownDialogueChecks::RunTest(const FString&)
{
    return RunTownDialogue(*this, 2, TEXT("NAR-0007"), TEXT("NAR-0003"), TEXT("B01MixedTown"));
}
#endif
