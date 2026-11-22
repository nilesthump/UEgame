#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS
#include "ExperimentAdapter/ExperimentSubsystem.h"
#include "ExperimentAdapter/ExperimentWorldDirector.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "HAL/PlatformTime.h"
#include "InputCoreTypes.h"
#include "InputKeyEventArgs.h"
#include "Tests/AutomationCommon.h"
#include "UnrealClient.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBootstrapTownDialogueChecks, "UEgame.WorldScenario.B01.TownDialogue",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::NonNullRHI | EAutomationTestFlags::ProductFilter)

bool FBootstrapTownDialogueChecks::RunTest(const FString&)
{
    UWorld* World = nullptr;
    for (const auto& Context : GEngine->GetWorldContexts())
        if (Context.WorldType == EWorldType::Game) World = Context.World();
    if (!TestNotNull(TEXT("game world"), World)) return false;
    TActorIterator<AExperimentWorldDirector> Actors(World);
    if (!TestTrue(TEXT("director exists"), bool(Actors))) return false;
    const TWeakObjectPtr<AExperimentWorldDirector> Director(*Actors);
    const TWeakObjectPtr<APlayerController> Controller(World->GetFirstPlayerController());
    const TWeakObjectPtr<UExperimentSubsystem> Adapter(World->GetGameInstance()->GetSubsystem<UExperimentSubsystem>());
    if (!TestTrue(TEXT("rescue ready"), Controller.IsValid() && Adapter.IsValid() &&
        Adapter->View().IsSet() && Adapter->View()->phase == experiment::ExperimentPhase::Rescue)) return false;

    auto Press = [this, Controller](FKey Key, int Count)
    {
        for (int Index = 0; Index < Count; ++Index)
        {
            ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Controller, Key]()
            {
                if (!TestTrue(TEXT("controller available"), Controller.IsValid())) return true;
                for (const auto Event : {IE_Pressed, IE_Released})
                    Controller->InputKey(FInputKeyEventArgs(nullptr, FInputDeviceId::CreateFromInternalId(0),
                        Key, Event, FPlatformTime::Cycles64()));
                return true;
            }));
            ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.12f));
        }
    };
    auto Await = [this, Director, Adapter](const TCHAR* Description, TFunction<bool()> Condition)
    {
        ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
            [this, Director, Adapter, Description, Condition, Deadline = 0.0]() mutable
            {
                if (!Director.IsValid() || !Adapter.IsValid() || !Adapter->View().IsSet())
                {
                    AddError(TEXT("lost town binding"));
                    return true;
                }
                if (Condition()) return true;
                const double Now = FPlatformTime::Seconds();
                if (Deadline == 0.0) Deadline = Now + 20.0;
                if (Now < Deadline) return false;
                AddError(Description);
                return true;
            }));
    };
    Press(EKeys::E, 6);
    Await(TEXT("control never restored"), [Adapter] { return Adapter->View()->phase == experiment::ExperimentPhase::Revisit; });
    Press(EKeys::A, 4);
    Await(TEXT("crowd subtitle missing"), [Director] { return Director->GetActiveTownTextId() == TEXT("NAR-0001"); });
    Press(EKeys::D, 4);
    Await(TEXT("leaving crowd must clear its subtitle"), [Director] { return Director->GetActiveTownTextId().IsEmpty(); });
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Adapter]()
    {
        TestTrue(TEXT("partial exposure earns neither receipt"), Adapter.IsValid() && Adapter->View().IsSet() &&
            !Adapter->View()->crowdContentDelivered && !Adapter->View()->teacherRecallDelivered);
        return true;
    }));
    Press(EKeys::A, 4);
    Await(TEXT("crowd subtitle never restarted"), [Director] { return Director->GetActiveTownTextId() == TEXT("NAR-0001"); });
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.3f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([] { FScreenshotRequest::RequestScreenshot(TEXT("B01TownCrowd"), true, true); return true; }));
    Await(TEXT("crowd receipt missing"), [Adapter] { return Adapter->View()->crowdContentDelivered; });
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Adapter]()
    {
        TestTrue(TEXT("crowd alone leaves revisit active"), Adapter.IsValid() && Adapter->View().IsSet() &&
            Adapter->View()->phase == experiment::ExperimentPhase::Revisit && !Adapter->View()->teacherRecallDelivered);
        return true;
    }));
    Press(EKeys::D, 8);
    Await(TEXT("teacher subtitle missing"), [Director] { return Director->GetActiveTownTextId() == TEXT("NAR-0005"); });
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.3f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([] { FScreenshotRequest::RequestScreenshot(TEXT("B01TownTeacher"), true, true); return true; }));
    Await(TEXT("two delivered clips must end the run"), [Adapter] {
        const auto View = Adapter->View();
        return View->crowdContentDelivered && View->teacherRecallDelivered && View->phase == experiment::ExperimentPhase::Ended;
    });
    return true;
}
#endif
