// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CaravanAbilityDialogEditor : ModuleRules
{
	public CaravanAbilityDialogEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "EditorStyle", "InputCore", "NetCore", "UMG", "UnrealEd", "CaravanAbility" });

		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks", "Slate", "SlateCore", "BlueprintGraph"});
	}
}
