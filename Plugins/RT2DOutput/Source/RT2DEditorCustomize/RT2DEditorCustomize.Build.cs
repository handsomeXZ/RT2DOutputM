using UnrealBuildTool;

public class RT2DEditorCustomize : ModuleRules
{
    public RT2DEditorCustomize(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "AssetTools",
                "EditorStyle",
                "ToolMenus",
                "UnrealEd",
                "RT2DRuntimeCustomize",
                "RHI",
                "Renderer",
                "RenderCore",
                "Projects",
                "EditorFramework",
                "InputCore",
                "AssetRegistry",

                "SteamAudio",
                "SteamAudioEditor",
                "LevelEditor"

            });

    }
}