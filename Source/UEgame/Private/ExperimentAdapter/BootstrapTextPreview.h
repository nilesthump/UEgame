#pragma once
#include "CoreMinimal.h"

// Identity and metadata from the single accepted B01 narrative source.
struct FBootstrapTextEntry
{
    FString Id, Speaker, Scene, Text, Status;
};

bool LoadBootstrapTextEntry(const FString& Csv, const TCHAR* Id, const TCHAR* Scene, FBootstrapTextEntry& Entry);
bool LoadBootstrapTextPreview(const FString& Csv, FBootstrapTextEntry& Entry);
