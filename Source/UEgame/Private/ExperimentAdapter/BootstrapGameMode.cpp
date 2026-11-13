#include "BootstrapGameMode.h"
#include "ExperimentWorldDirector.h"
#include "Camera/CameraActor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DirectionalLight.h"

ABootstrapGameMode::ABootstrapGameMode()
{
    DefaultPawnClass = nullptr;
}

void ABootstrapGameMode::StartPlay()
{
    Super::StartPlay();
    UWorld* World = GetWorld();
    World->SpawnActor<ADirectionalLight>(FVector::ZeroVector, FRotator(0, 180, 0));
    World->SpawnActor<AExperimentWorldDirector>(FVector(0, 0, 100), FRotator::ZeroRotator);
    ACameraActor* Camera = World->SpawnActor<ACameraActor>(FVector(600, 0, 100), FRotator(0, 180, 0));
    if (APlayerController* Controller = World->GetFirstPlayerController(); Controller && Camera)
    {
        Controller->SetViewTarget(Camera);
    }
}
