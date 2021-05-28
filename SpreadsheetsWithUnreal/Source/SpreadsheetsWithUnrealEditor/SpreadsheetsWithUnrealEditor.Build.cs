// Some copyright should be here...

using UnrealBuildTool;

public class SpreadsheetsWithUnrealEditor : ModuleRules
{
	public SpreadsheetsWithUnrealEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "UnrealEd"
            });

        PublicDependencyModuleNames.AddRange(new string[] { "SpreadsheetsWithUnreal" });

        PublicIncludePaths.AddRange(
            new string[]
            {
                "SpreadsheetsWithUnrealEditor/Public"
            });

        PrivateIncludePaths.AddRange(
            new string[]
            {
                "SpreadsheetsWithUnrealEditor/Private"
            });

        if (Target.Type == TargetRules.TargetType.Editor)
        {
            PublicIncludePathModuleNames.AddRange(
                new string[] {
                    "Settings"
                }
            );

            DynamicallyLoadedModuleNames.AddRange(
                new string[] {
                    "Settings"
                }
            );
        }
    }
}

