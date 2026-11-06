using UnrealBuildTool;
public class UEgameTarget : TargetRules
{
    public UEgameTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
        WindowsPlatform.Compiler = WindowsCompiler.VisualStudio2022;
        WindowsPlatform.CompilerVersion = "14.38.33130";
        WindowsPlatform.WindowsSdkVersion = "10.0.22621.0";
        ExtraModuleNames.Add("UEgame");
    }
}
