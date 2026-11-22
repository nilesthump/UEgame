#include "Misc/AutomationTest.h"
#if WITH_DEV_AUTOMATION_TESTS
#include "ExperimentAdapter/BootstrapTextPreview.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBootstrapTextChecks, "UEgame.B01.DraftTextSource",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)
bool FBootstrapTextChecks::RunTest(const FString&)
{
    FString Csv;
    FBootstrapTextEntry Entry;
    if (!TestTrue(TEXT("canonical source available"), FFileHelper::LoadFileToString(Csv,
        *(FPaths::ProjectDir() / TEXT("spec/contracts/narrative/TEXT.csv"))))) return false;
    TestTrue(TEXT("read accepted draft entry"), LoadBootstrapTextPreview(Csv, Entry));
    TestEqual(TEXT("stable identity retained"), Entry.Id, FString(TEXT("NAR-0004")));
    TestEqual(TEXT("status retained"), Entry.Status, FString(TEXT("draft")));
    TestFalse(TEXT("missing source fails closed"), LoadBootstrapTextPreview(TEXT(""), Entry));
    TestFalse(TEXT("retired source fails closed"), LoadBootstrapTextPreview(Csv.Replace(TEXT(",draft"), TEXT(",retired")), Entry));
    TestFalse(TEXT("duplicate source fails closed"), LoadBootstrapTextPreview(Csv + Csv, Entry));
    const FString DraftCsv = Csv.Replace(TEXT(",final"), TEXT(",draft"));
    for (const TCHAR* SourceStatus : {TEXT("draft"), TEXT("final")})
    {
        const FString Source = DraftCsv.Replace(TEXT(",draft"), *(FString(TEXT(",")) + SourceStatus));
        TestTrue(TEXT("active revisit line imports"), LoadBootstrapTextEntry(Source, TEXT("NAR-0001"), TEXT("revisit.crowd.sword"), Entry));
        TestEqual(TEXT("revisit source status preserved"), Entry.Status, FString(SourceStatus));
    }
    TestFalse(TEXT("retired teacher line cannot start a job"), LoadBootstrapTextEntry(
        DraftCsv.Replace(TEXT(",draft"), TEXT(",retired")), TEXT("NAR-0005"), TEXT("revisit.teacher.sword"), Entry));
    TestFalse(TEXT("mismatched town role rejected"), LoadBootstrapTextEntry(
        Csv, TEXT("NAR-0001"), TEXT("revisit.teacher.sword"), Entry));
    return true;
}
#endif
