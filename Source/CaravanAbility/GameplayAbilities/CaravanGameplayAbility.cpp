// Fill out your copyright notice in the Description page of Project Settings.


#include "CaravanGameplayAbility.h"

void UCaravanGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	UE_LOG(LogTemp, Display, TEXT("Gameplay ability granted."));
}
