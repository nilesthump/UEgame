using UnrealBuildTool;
using System.IO;
public class UEgame : ModuleRules
{
    public UEgame(ReadOnlyTargetRules Target) : base(Target)
    {
        CppStandard = CppStandardVersion.Cpp20;
        bEnableExceptions = false;
        bUseRTTI = false;
        PCHUsage = PCHUsageMode.NoPCHs;
        bUseUnity = false;
        PrivateDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine" });
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/ExperimentCore/include"));
        PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "../../Tests/Shared")));
    }
}
