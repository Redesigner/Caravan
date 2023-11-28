// Copyright (c) 2023 Stephen Melnick

#include "CaravanAbility/GameplayAbilities/Abilities/CaravanGameplayAbility.h"

void UCaravanGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	UE_LOG(LogTemp, Display, TEXT("Gameplay ability granted."));
}
