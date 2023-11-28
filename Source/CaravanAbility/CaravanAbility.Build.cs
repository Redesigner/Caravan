// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CaravanAbility : ModuleRules
{
	public CaravanAbility(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", "InputCore",
			"NetCore", "UMG",
			"CaravanGame"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"Json", "JsonUtilities", "GameplayAbilities", "GameplayTags",
			"GameplayTasks", "Slate", "SlateCore"
		});
	}
}
