// Copyright (c) 2023 Stephen Melnick

#include "CaravanAbility.h"

#include "Modules/ModuleManager.h"
#include "GameplayDebugger.h"
#include "GameFramework/HUD.h"

#include "CaravanAbility/System/GameplayDebuggerCategory_CaravanAbility.h"
#include "CaravanAbility/GameplayAbilities/Components/CharacterAbilitySystemComponent.h"

DEFINE_LOG_CATEGORY(LogCaravanAbility);
DEFINE_LOG_CATEGORY(LogAbilityQueue);
DEFINE_LOG_CATEGORY(LogAbilityCombos);

DEFINE_LOG_CATEGORY(LogTargetingReticleSystem);

IMPLEMENT_GAME_MODULE(FCaravanAbilityModule, CaravanAbility);

void FCaravanAbilityModule::StartupModule()
{
	UE_LOG(LogCaravanAbility, Display, TEXT("CaravanAbility: Module Started"));

#if WITH_GAMEPLAY_DEBUGGER
	IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
	GameplayDebuggerModule.RegisterCategory("CaravanAbility",
		IGameplayDebugger::FOnGetCategory::CreateStatic(&FGameplayDebuggerCategory_CaravanAbility::MakeInstance)
	);
	GameplayDebuggerModule.NotifyCategoriesChanged();
#endif

	/*if (!IsRunningDedicatedServer())
	{
		AHUD::OnShowDebugInfo.AddStatic(&UCharacterAbilitySystemComponent::OnShowDebugInfo);
	}*/
}

void FCaravanAbilityModule::ShutdownModule()
{
#if WITH_GAMEPLAY_DEBUGGER
	if (IGameplayDebugger::IsAvailable())
	{
		IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
		GameplayDebuggerModule.UnregisterCategory("CaravanAbility");
		GameplayDebuggerModule.NotifyCategoriesChanged();
	}
#endif
}
