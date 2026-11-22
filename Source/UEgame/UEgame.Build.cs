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
        PrivateDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "InputCore", "Slate", "SlateCore" });
        RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "../../spec/contracts/narrative/TEXT.csv"), StagedFileType.UFS);
        if (Target.Type == TargetType.Game && Target.Configuration == UnrealTargetConfiguration.Development)
        {
            RuntimeDependencies.Add("$(EngineDir)/Content/Automation/Report-Template.html", StagedFileType.UFS);
        }
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/ExperimentCore/include"));
        PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "../../Tests/Shared")));
    }
}
