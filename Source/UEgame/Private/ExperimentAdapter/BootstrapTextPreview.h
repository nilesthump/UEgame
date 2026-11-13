#pragma once
#include "CoreMinimal.h"

// Read-only B01 display probe; never reports narrative delivery to Session.
struct FBootstrapTextEntry
{
    FString Id, Speaker, Scene, Text, Status;
};

bool LoadBootstrapTextPreview(const FString& Csv, FBootstrapTextEntry& Entry);
