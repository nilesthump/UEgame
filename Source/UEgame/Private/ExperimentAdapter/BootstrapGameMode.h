#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BootstrapGameMode.generated.h"

// Minimal B01 host; this is not the encounter or player implementation.
UCLASS()
class ABootstrapGameMode final : public AGameModeBase
{
    GENERATED_BODY()
public:
    ABootstrapGameMode();
    virtual void StartPlay() override;
};
