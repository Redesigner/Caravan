// Copyright (c) 2023 Stephen Melnick


#include "CaravanGame/Character/CharacterBaseAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h" 

UCharacterBaseAttributeSet::UCharacterBaseAttributeSet()
{
	MovementSpeed.SetBaseValue(600.0f);
	MovementSpeed.SetCurrentValue(600.0f);

	CurrentHealth.SetBaseValue(100.0f);
	CurrentHealth.SetCurrentValue(100.0f);
}

void UCharacterBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterBaseAttributeSet, CurrentHealth, COND_None, REPNOTIFY_Always);
}

void UCharacterBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute.AttributeName == TEXT("CurrentHealth"))
	{
		if (Data.EvaluatedData.Attribute.GetNumericValue(this) <= 0.0f)
		{
			OnDeath.ExecuteIfBound(Data.EffectSpec);
		}
	}
}

void UCharacterBaseAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterBaseAttributeSet, CurrentHealth, OldCurrentHealth);
}

