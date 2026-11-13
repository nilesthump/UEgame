#include "ExperimentWorldDirector.h"
#include "ExperimentSubsystem.h"
#include "Components/TextRenderComponent.h"
#include "Engine/GameInstance.h"

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
                    StatusText->SetText(FText::FromString(TEXT("B01: bootstrap ready")));
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
    BindingRun = 0;
    Adapter.Reset();
    Super::EndPlay(Reason);
}
