// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBaseMovementComponent.h"

#include "CharacterBase.h"
#include "CaravanAbility/GameplayAbilities/Components/CharacterAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"

UCharacterBaseMovementComponent::UCharacterBaseMovementComponent()
{
	bIsStunned = false;
}


void UCharacterBaseMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterBaseMovementComponent, bIsStunned, COND_OwnerOnly, REPNOTIFY_Always);
}


float UCharacterBaseMovementComponent::GetMaxSpeed() const
{
	if (bIsStunned)
	{
		return 0.0f;
	}
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


void UCharacterBaseMovementComponent::PhysicsRotation(float DeltaTime)
{
	if (HasRootMotionSources())
	{
		// Temporary workaround, so that all root motion blocks rotation, not just animation based
		return;
	}
	Super::PhysicsRotation(DeltaTime);
}


void UCharacterBaseMovementComponent::DisableMovementWithStun(float Time)
{
	const FDateTime CurrentTime = FDateTime::Now();
	int TimeMS = CurrentTime.GetMillisecond();
	UE_LOG(LogTemp, Display, TEXT("Local Character Movement paused at %i"), TimeMS);

	bIsStunned = true;
	GetWorld()->GetTimerManager().SetTimer(StunTimer, FTimerDelegate::CreateLambda(
		[this]() {
			float TimeElapsed = GetWorld()->GetTimerManager().GetTimerElapsed(StunTimer);
			bIsStunned = false;
		}), Time, false);
}


void UCharacterBaseMovementComponent::ReplicateStun(bool bStunState)
{
	bIsStunned = bStunState;
}
