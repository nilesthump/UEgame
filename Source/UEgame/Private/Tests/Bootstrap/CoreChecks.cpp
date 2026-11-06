#include "Misc/AutomationTest.h"
#if WITH_DEV_AUTOMATION_TESTS
#include "CoreCaseCatalog.h"
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWorldReadyCheck, "UEgame.B01.Session.WorldReady",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FWorldReadyCheck::RunTest(const FString& Parameters) {
    return TestTrue(TEXT("Shared world readiness boundary"), experiment::checks::WorldReadyBoundary());
}
#endif
