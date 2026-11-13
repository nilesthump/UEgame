#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BootstrapMarker.generated.h"
class UTextRenderComponent;

// B01-only visible fixture state, not combat health or a production participant.
UCLASS()
class ABootstrapMarker final : public AActor
{
    GENERATED_BODY()
public:
    ABootstrapMarker();
    void InitializeMarker(const FString& Label, bool Alive);
    bool IsAlive() const { return bAlive; }
    bool IsReady() const;
private:
    UPROPERTY() TObjectPtr<UTextRenderComponent> LabelText;
    bool bAlive = true;
};
