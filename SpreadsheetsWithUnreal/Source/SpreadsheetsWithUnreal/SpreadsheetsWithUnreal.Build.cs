// Some copyright should be here...

using UnrealBuildTool;

public class SpreadsheetsWithUnreal : ModuleRules
{
	public SpreadsheetsWithUnreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "HTTP",
                "Json",
                "JsonUtilities",
                "Slate",
                "SlateCore"
            });

        PublicDependencyModuleNames.AddRange(new string[] { "SpreadsheetsWithUnrealEditor" });

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

