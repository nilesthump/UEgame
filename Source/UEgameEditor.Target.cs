using UnrealBuildTool;
public class UEgameEditorTarget : TargetRules
{
    public UEgameEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
        WindowsPlatform.Compiler = WindowsCompiler.VisualStudio2022;
        WindowsPlatform.CompilerVersion = "14.38.33130";
        WindowsPlatform.WindowsSdkVersion = "10.0.22621.0";
        ExtraModuleNames.Add("UEgame");
    }
}
