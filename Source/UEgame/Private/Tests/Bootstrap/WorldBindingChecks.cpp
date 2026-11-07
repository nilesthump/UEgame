#include "Misc/AutomationTest.h"
#if WITH_DEV_AUTOMATION_TESTS
#include "ExperimentAdapter/ExperimentSubsystem.h"
#include "ExperimentAdapter/ExperimentWorldDirector.h"
#include "Components/SceneComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBootstrapBindingChecks, "UEgame.B01.Bootstrap.WorldBinding",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FBootstrapBindingChecks::RunTest(const FString&)
{
    UGameInstance* Instance = NewObject<UGameInstance>(GEngine);
    Instance->InitializeStandalone();
    UWorld* World = Instance->GetWorld();
    auto* Adapter = Instance->GetSubsystem<UExperimentSubsystem>();
    auto* First = World->SpawnActor<AExperimentWorldDirector>();
    auto* Second = World->SpawnActor<AExperimentWorldDirector>();
    const auto Run = Adapter->BindWorld(First);
    TestTrue(TEXT("actual game world binds"), Run != 0);
    TestTrue(TEXT("clock waits for actual assembly receipt"), !Adapter->View()->worldStartedAt.has_value());
    TestEqual(TEXT("second director cannot replace active binding"), Adapter->BindWorld(Second), experiment::RunId{0});
    First->GetRootComponent()->UnregisterComponent();
    TestFalse(TEXT("unregistered world component cannot report ready"), Adapter->NotifyBootstrapReady(First, Run));
    First->GetRootComponent()->RegisterComponent();
    TestTrue(TEXT("registered assembly receipt accepted"), Adapter->NotifyBootstrapReady(First, Run));
    TestTrue(TEXT("Session starts from real world receipt"), Adapter->View()->worldStartedAt.has_value());
    TestFalse(TEXT("duplicate ready rejected"), Adapter->NotifyBootstrapReady(First, Run));
    Adapter->UnbindWorld(Second);
    TestTrue(TEXT("foreign unbind preserves binding"), Adapter->IsCurrentBinding(First, Run));
    Adapter->UnbindWorld(First);
    TestFalse(TEXT("unload invalidates late callback"), Adapter->NotifyBootstrapReady(First, Run));
    const auto NextRun = Adapter->BindWorld(Second);
    TestTrue(TEXT("replacement has new run identity"), NextRun > Run);
    TestFalse(TEXT("old identity cannot stamp new world"), Adapter->NotifyBootstrapReady(Second, Run));
    TestTrue(TEXT("replacement waits for its receipt"), !Adapter->View()->worldStartedAt.has_value());
    TestTrue(TEXT("replacement ready"), Adapter->NotifyBootstrapReady(Second, NextRun));
    Adapter->UnbindWorld(Second);
    auto* Automatic = World->SpawnActor<AExperimentWorldDirector>();
    Automatic->DispatchBeginPlay();
    const auto AutomaticRun = Adapter->View()->runId;
    TestTrue(TEXT("BeginPlay binds and submits real assembly receipt"),
        AutomaticRun > NextRun && Adapter->View()->worldStartedAt.has_value());
    World->DestroyActor(Automatic);
    TestFalse(TEXT("EndPlay removes binding"), Adapter->IsCurrentBinding(Automatic, AutomaticRun));
    UGameInstance* OtherInstance = NewObject<UGameInstance>(GEngine);
    OtherInstance->InitializeStandalone();
    UWorld* OtherWorld = OtherInstance->GetWorld();
    auto* Foreign = OtherWorld->SpawnActor<AExperimentWorldDirector>();
    TestEqual(TEXT("foreign game instance world rejected"), Adapter->BindWorld(Foreign), experiment::RunId{0});
    OtherInstance->Shutdown();
    GEngine->DestroyWorldContext(OtherWorld);
    OtherWorld->DestroyWorld(false);
    Instance->Shutdown();
    GEngine->DestroyWorldContext(World);
    World->DestroyWorld(false);
    return true;
}
#endif
