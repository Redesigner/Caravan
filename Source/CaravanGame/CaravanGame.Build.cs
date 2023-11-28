// Copyright (c) 2023 Stephen Melnick

using UnrealBuildTool;

public class CaravanGame : ModuleRules
{
	public CaravanGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore",
            "NetCore", "UMG", "CaravanAbility" });

		PrivateDependencyModuleNames.AddRange(new string[] {
            "Json", "JsonUtilities", "GameplayAbilities", "GameplayTags",
            "GameplayTasks", "Slate", "SlateCore"
		});
    }
}