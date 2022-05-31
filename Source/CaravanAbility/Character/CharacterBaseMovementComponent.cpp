// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBaseMovementComponent.h"

#include "CharacterBase.h"
#include "CaravanAbility/GameplayAbilities/Components/CharacterAbilitySystemComponent.h"
#include "GameFramework/Character.h"

UCharacterBaseMovementComponent::UCharacterBaseMovementComponent()
{
	bIsStunned = false;
}

void UCharacterBaseMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunction)
{
	Super::TickComponent(DeltaTime, TickType, TickFunction);
	if (bIsStunned && GetOwnerRole() == ENetRole::ROLE_AutonomousProxy)
	{
		float StunRemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(StunTimer);
		float StunElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(StunTimer);
		if (StunRemainingTime <= DeltaTime)
		{
			UE_LOG(LogTemp, Display, TEXT("Timer would end mid-tick -- forcing the stun to end early, at %f"), StunElapsedTime);
			// GetWorld()->GetTimerManager().ClearTimer(StunTimer);
			// bIsStunned = false;
		}
	}
}

float UCharacterBaseMovementComponent::GetMaxSpeed() const
{
	if (MovementMode == EMovementMode::MOVE_Walking)
	{
		/* ACharacter* Owner = GetCharacterOwner();
		if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(Owner))
		{
			bool bSucceeded = false;
			const float MovementSpeedValue = CharacterBase->GetAbilitySystemComponent()->GetGameplayAttributeValue(MovementSpeed, bSucceeded);
			if (bSucceeded)
			{
				return MovementSpeedValue;
			}
		} */
		if (bIsStunned)
		{
			return 0.0f;
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

void UCharacterBaseMovementComponent::ReplicateMoveToServer(float DeltaTime, const FVector& NewAcceleration)
{
	if (bIsStunned)
	{
		Super::ReplicateMoveToServer(DeltaTime, FVector::Zero());
		return;
	}
	Super::ReplicateMoveToServer(DeltaTime, NewAcceleration);
}

void UCharacterBaseMovementComponent::DisableMovementWithStun(float Time)
{
	StopMovementImmediately();
	AActor* Owner = GetOwner();
	ENetRole Role = GetOwnerRole();
	if (Role == ENetRole::ROLE_AutonomousProxy)
	{
		ForceClientAdjustment();

		const FDateTime CurrentTime = FDateTime::Now();
		int TimeMS = CurrentTime.GetMillisecond();
		UE_LOG(LogTemp, Display, TEXT("Local Character Movement paused at %i"), TimeMS);

		bIsStunned = true;

		GetWorld()->GetTimerManager().SetTimer(StunTimer, FTimerDelegate::CreateLambda(
			[this]() {
				const FDateTime CurrentTime = FDateTime::Now();
				int TimeMS = CurrentTime.GetMillisecond();
				float TimeElapsed = GetWorld()->GetTimerManager().GetTimerElapsed(StunTimer);
				UE_LOG(LogTemp, Display, TEXT("Local Character Movement resumed at %i, after %f."), TimeMS, TimeElapsed);

				bIsStunned = false;
			}), Time, false);
	}
	else if (Role == ENetRole::ROLE_Authority)
	{
		FlushServerMoves();

		const FDateTime CurrentTime = FDateTime::Now();
		int TimeMS = CurrentTime.GetMillisecond();
		UE_LOG(LogTemp, Display, TEXT("Character Movement paused at %i"), TimeMS);

		bIsStunned = true;

		GetWorld()->GetTimerManager().SetTimer(StunTimer, FTimerDelegate::CreateLambda(
			[this]() {
				const FDateTime CurrentTime = FDateTime::Now();
				int TimeMS = CurrentTime.GetMillisecond();
				float TimeElapsed = GetWorld()->GetTimerManager().GetTimerElapsed(StunTimer);
				UE_LOG(LogTemp, Display, TEXT("Character Movement resumed at %i, after %f."), TimeMS, TimeElapsed);

				bIsStunned = false;
			}), Time, false);
	}
}
