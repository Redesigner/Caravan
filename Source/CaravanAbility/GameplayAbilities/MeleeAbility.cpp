// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAbility.h"

#include "CaravanAbility/GameplayAbilities/Components/CharacterAbilitySystemComponent.h"
#include "CaravanAbility/Character/CharacterBase.h"
#include "CaravanAbility/Character/Components/CharacterBaseMovementComponent.h"

UMeleeAbility::UMeleeAbility()
{
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnlyTermination;
	bReplicateInputDirectly = true;
	bRetriggerInstancedAbility = true;
}


void UMeleeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ExecuteAttack(Handle, ActorInfo, ActivationInfo);
}


void UMeleeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UMeleeAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
}


void UMeleeAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ExecuteAttack(Handle, ActorInfo, ActivationInfo, false);
}


void UMeleeAbility::ExecuteAttack(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool InitialAttack)
{
	// If we don't have an animation, this ability won't work...
	if (!MeleeAnimation)
	{
		return;
	}
	if (InitialAttack)
	{
		MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("AnimationMontageTask"), MeleeAnimation, 1.0f, MontageSectionName, false);
		// Make sure to call the end ability function at the end of the ability, or cooldowns etc. won't work
		MontageTask->OnBlendOut.AddDynamic(this, &UMeleeAbility::EndAbilityManual);
		// If the ability gets interrupted, manually call it as well.
		// TODO: The abilities shouldn't be interrupted, but it *can* happen currently
		MontageTask->OnInterrupted.AddDynamic(this, &UMeleeAbility::EndAbilityManual);

		MontageTask->ReadyForActivation();

		// Bind our hit functions to the hitbox controller
		if (UActorComponent* ActorComponent = ActorInfo->AvatarActor->GetComponentByClass(UHitboxController::StaticClass()))
		{
			UHitboxController* HitboxController = Cast<UHitboxController>(ActorComponent);
			if (HasAuthority(&ActivationInfo))
			{
				HitboxController->HitDetectedDelegate.BindUObject(this, &UMeleeAbility::HitTarget);
				HitboxController->HitInterruptedDelegate.BindUObject(this, &UMeleeAbility::HitInterrupted);
			}
			else
			{
				// Even if we don't have authority, still call the blueprint hit function, so we can apply any cues we need
				HitboxController->HitDetectedDelegate.BindUObject(this, &UMeleeAbility::HitTargetLocal);
			}
			HitboxController->ClearOverlaps();
		}
	}
	if (UCharacterAbilitySystemComponent* CharacterASC = Cast<UCharacterAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
	{
		CharacterASC->SetNextComboAbility(NextComboState);
	}
}


void UMeleeAbility::EndAbilityManual()
{
	UE_LOG(LogAbilitySystem, Display, TEXT("[%s] Ability ended Manually."), HasAuthority(&CurrentActivationInfo) ? TEXT("Authority") : TEXT("Proxy") );
	if (UCharacterAbilitySystemComponent* CharacterASC = Cast<UCharacterAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
	{
		CharacterASC->SetNextComboAbility(TEXT("NONE"));
	}
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}


void UMeleeAbility::HitTarget(const FHitResult& HitResult)
{
	OnTargetHit(HitResult);
	ApplyGameplayCue(HitResult);

	if (HitEffect)
	{
		const FGameplayEffectSpecHandle HitEffectSpec = MakeOutgoingGameplayEffectSpec(HitEffect);
		FGameplayAbilityTargetDataHandle TargetDataHandle;
	
		TargetDataHandle.Add(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));
		
		ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
			HitEffectSpec, TargetDataHandle);
	}
}


void UMeleeAbility::HitTargetLocal(const FHitResult& HitResult)
{
	OnTargetHit(HitResult);
	ApplyGameplayCue(HitResult);
}


void UMeleeAbility::ApplyGameplayCue(const FHitResult& HitResult)
{
	AActor* Owner = GetOwningActorFromActorInfo();

	if (UCharacterAbilitySystemComponent* CharacterASC = Cast<UCharacterAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
	{
		FGameplayAbilityTargetDataHandle TargetDataHandle;
		TargetDataHandle.Add(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));

		FGameplayCueParameters GameplayCueParams = FGameplayCueParameters();
		GameplayCueParams.EffectContext = GetContextFromOwner(TargetDataHandle);
		GameplayCueParams.Location = HitResult.Location;
		GameplayCueParams.Normal = HitResult.Normal;

		if (Owner->GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
		{
			// The ability was predicted, so apply the cue locally
			GameplayCueParams.Instigator = Owner->GetOwner();
			CharacterASC->ExecuteGameplayCueLocal(GameplayCueSelf, GameplayCueParams);
		}
		else
		{
			GameplayCueParams.Instigator = nullptr;
			CharacterASC->ExecuteGameplayCue(GameplayCueSelf, GameplayCueParams);
		}
	}
}

void UMeleeAbility::HitInterrupted(const FHitResult& HitResult)
{
	MontageTask->EndTask();
	MontageStop();
	EndAbilityManual();
}
