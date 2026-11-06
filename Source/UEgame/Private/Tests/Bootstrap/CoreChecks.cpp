#include "Misc/AutomationTest.h"
#if WITH_DEV_AUTOMATION_TESTS
#include "CoreCaseCatalog.h"
IMPLEMENT_COMPLEX_AUTOMATION_TEST(FSessionCoreChecks, "UEgame.B01.Session",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
void FSessionCoreChecks::GetTests(TArray<FString>& Names, TArray<FString>& Commands) const {
    for (const auto& Test : experiment::checks::Catalog) {
        const FString Id(UTF8_TO_TCHAR(Test.id.data()));
        Names.Add(Id.RightChop(12)); // Strip "B01.Session." from the leaf label.
        Commands.Add(Id);
    }
}
bool FSessionCoreChecks::RunTest(const FString& Parameters) {
    for (const auto& Test : experiment::checks::Catalog)
        if (Parameters == UTF8_TO_TCHAR(Test.id.data()))
            return TestTrue(*Parameters, Test.run());
    AddError(TEXT("Unknown shared CaseId"));
    return false;
}
#endif
