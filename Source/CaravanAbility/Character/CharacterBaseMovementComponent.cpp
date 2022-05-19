// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBaseMovementComponent.h"

#include "CharacterBase.h"
#include "CaravanAbility/GameplayAbilities/CharacterAbilitySystemComponent.h"
#include "GameFramework/Character.h"

float UCharacterBaseMovementComponent::GetMaxSpeed() const
{
	if (MovementMode == EMovementMode::MOVE_Walking)
	{
		ACharacter* Owner = GetCharacterOwner();
		if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(Owner))
		{
			bool bSucceeded = false;
			const float MovementSpeedValue = CharacterBase->GetAbilitySystemComponent()->GetGameplayAttributeValue(MovementSpeed, bSucceeded);
			if (bSucceeded)
			{
				return MovementSpeedValue;
			}
		}

	}
	return Super::GetMaxSpeed();
}

FRotator UCharacterBaseMovementComponent::GetDeltaRotation(float DeltaTime) const
{
	if (MovementMode == EMovementMode::MOVE_Walking)
	{
		ACharacter* Owner = GetCharacterOwner();
		if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(Owner))
		{
			bool bSucceeded = false;
			const float MovementSpeedValue = CharacterBase->GetAbilitySystemComponent()->GetGameplayAttributeValue(MovementSpeed, bSucceeded);
			if (bSucceeded)
			{
				return RotationRate * (MovementSpeedValue / 600.0f) * DeltaTime;
			}
		}
	}
	return Super::GetDeltaRotation(DeltaTime);
}
