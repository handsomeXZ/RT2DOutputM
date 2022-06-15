using UnrealBuildTool;

public class RT2DRuntimeCustomize : ModuleRules
{
    public RT2DRuntimeCustomize(ReadOnlyTargetRules Target) : base(Target)
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
                "RHI",
                "RenderCore",
                "Renderer",
                "Projects",
                "EditorFramework",
                "InputCore"
            });


    }
}