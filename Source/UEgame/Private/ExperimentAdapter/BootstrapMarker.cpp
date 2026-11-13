#include "BootstrapMarker.h"
#include "Components/TextRenderComponent.h"
ABootstrapMarker::ABootstrapMarker()
{
    LabelText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Marker"));
    SetRootComponent(LabelText);
    LabelText->SetWorldSize(20.f);
    LabelText->SetHorizontalAlignment(EHTA_Center);
}
void ABootstrapMarker::InitializeMarker(const FString& Label, bool Alive)
{
    bAlive = Alive;
    LabelText->SetText(FText::FromString(Label + (Alive ? TEXT(": alive") : TEXT(": dead"))));
    LabelText->SetTextRenderColor(Alive ? FColor::Green : FColor::Red);
}
bool ABootstrapMarker::IsReady() const
{
    return !IsActorBeingDestroyed() && LabelText && LabelText->IsRegistered();
}
