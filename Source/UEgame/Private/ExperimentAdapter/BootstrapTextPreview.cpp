#include "BootstrapTextPreview.h"
#include "Serialization/Csv/CsvParser.h"

bool LoadBootstrapTextEntry(const FString& Csv, const TCHAR* Id, const TCHAR* Scene, FBootstrapTextEntry& Entry)
{
    Entry = {};
    FCsvParser Parser(Csv);
    const auto& Rows = Parser.GetRows();
    const TCHAR* Header[] = {TEXT("id"), TEXT("speaker"), TEXT("scene_or_result"), TEXT("text"), TEXT("production_notes"), TEXT("status")};
    if (Rows.IsEmpty() || Rows[0].Num() != 6) return false;
    for (int32 Column = 0; Column < 6; ++Column)
        if (FCString::Strcmp(Rows[0][Column], Header[Column]) != 0) return false;
    FBootstrapTextEntry Match;
    bool bFound = false;
    for (int32 Index = 1; Index < Rows.Num(); ++Index)
    {
        const auto& Row = Rows[Index];
        if (Row.Num() != 6) return false;
        if (FCString::Strcmp(Row[0], Id) != 0) continue;
        if (bFound || FString(Row[1]).IsEmpty() || FString(Row[3]).IsEmpty() ||
            FCString::Strcmp(Row[2], Scene) != 0 ||
            (FCString::Strcmp(Row[5], TEXT("draft")) != 0 && FCString::Strcmp(Row[5], TEXT("final")) != 0)) return false;
        Match = {Row[0], Row[1], Row[2], Row[3], Row[5]};
        bFound = true;
    }
    if (bFound) Entry = MoveTemp(Match);
    return bFound;
}

bool LoadBootstrapTextPreview(const FString& Csv, FBootstrapTextEntry& Entry)
{
    if (LoadBootstrapTextEntry(Csv, TEXT("NAR-0004"), TEXT("rescue.failure.child-dead"), Entry) &&
        Entry.Status == TEXT("draft")) return true;
    Entry = {};
    return false;
}
