// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CaravanAbility : ModuleRules
{
	public CaravanAbility(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NetCore", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Json", "JsonUtilities", "GameplayAbilities", "GameplayTags", "GameplayTasks", "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
