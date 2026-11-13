#include "BootstrapTextPreview.h"
#include "Serialization/Csv/CsvParser.h"

bool LoadBootstrapTextPreview(const FString& Csv, FBootstrapTextEntry& Entry)
{
    Entry = {};
    FCsvParser Parser(Csv);
    const auto& Rows = Parser.GetRows();
    const TCHAR* Header[] = {TEXT("id"), TEXT("speaker"), TEXT("scene_or_result"), TEXT("text"), TEXT("production_notes"), TEXT("status")};
    if (Rows.IsEmpty() || Rows[0].Num() != 6) return false;
    for (int32 Column = 0; Column < 6; ++Column)
        if (FCString::Strcmp(Rows[0][Column], Header[Column]) != 0) return false;
    bool bFound = false;
    for (int32 Index = 1; Index < Rows.Num(); ++Index)
    {
        const auto& Row = Rows[Index];
        if (Row.Num() != 6) return false;
        if (FCString::Strcmp(Row[0], TEXT("NAR-0004")) != 0) continue;
        if (bFound || FString(Row[1]).IsEmpty() || FString(Row[3]).IsEmpty() ||
            FCString::Strcmp(Row[2], TEXT("rescue.failure.child-dead")) != 0 ||
            FCString::Strcmp(Row[5], TEXT("draft")) != 0) return false;
        Entry = {Row[0], Row[1], Row[2], Row[3], Row[5]};
        bFound = true;
    }
    return bFound;
}
