// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CaravanDialogEditor : ModuleRules
{
	public CaravanDialogEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", "EditorStyle",
			"InputCore", "NetCore", "UMG", "UnrealEd",
			"CaravanAbility", "CaravanGame"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"GameplayAbilities", "GameplayTags", "GameplayTasks", "Slate",
			"SlateCore", "BlueprintGraph"
		});
	}
}
