// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAbility.h"

#include "AbilitySystemComponent.h"
#include "Tasks/PlayMontageTask.h"
#include "Abilities/Tasks/AbilityTask_NetworkSyncPoint.h"
#include "CaravanAbility/Character/CharacterBase.h"
#include "CaravanAbility/Character/CharacterBaseMovementComponent.h"

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
	FName MontageSectionName = InitialComboState;
	if (!InitialAttack)
	{
		HandleMontageEnd();
		if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(ActorInfo->AvatarActor))
		{
			MontageSectionName = CharacterBase->GetComboState();
			// UE_LOG(LogAbilitySystem, Display, TEXT("[%s] Melee Combo Hit '%s' activated."), HasAuthority(&ActivationInfo) ? TEXT("Authority") : TEXT("Proxy"), *MontageSectionName.ToString());
		}
		if (HasAuthority(&ActivationInfo))
		{
			if (MeleeAnimation->IsValidSectionName(MontageSectionName))
			{
				MontageSetNextSectionName(InitialComboState, MontageSectionName);
				if (UActorComponent* ActorComponent = ActorInfo->AvatarActor->GetComponentByClass(UHitboxController::StaticClass()))
				{
					UHitboxController* HitboxController = Cast<UHitboxController>(ActorComponent);
					HitboxController->HitDetectedDelegate.BindUObject(this, &UMeleeAbility::HitTarget);
				}
			}
		}
		return;
	}
	// Check if the next section exists in the montage
	if (!MeleeAnimation->IsValidSectionName(MontageSectionName))
	{
		return;
	}
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("AnimationMontageTask"), MeleeAnimation, 1.0f, MontageSectionName, false);

	if (HasAuthority(&ActivationInfo))
	{
		Task->OnCompleted.AddDynamic(this, &UMeleeAbility::HandleMontageEnd);
		if (UActorComponent* ActorComponent = ActorInfo->AvatarActor->GetComponentByClass(UHitboxController::StaticClass()))
		{
			UHitboxController* HitboxController = Cast<UHitboxController>(ActorComponent);
			HitboxController->HitDetectedDelegate.BindUObject(this, &UMeleeAbility::HitTarget);
		}
	}
	Task->OnCompleted.AddDynamic(this, &UMeleeAbility::EndAbilityManual);

	Task->ReadyForActivation();
}


void UMeleeAbility::HandleMontageEnd()
{
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	UHitboxController* HitboxController = AvatarActor ? AvatarActor->FindComponentByClass<UHitboxController>() : nullptr;
	if (HitboxController != nullptr)
	{
		HitboxController->ClearOverlaps();
	}
}


void UMeleeAbility::EndAbilityManual()
{
	// UE_LOG(LogAbilitySystem, Display, TEXT("Ability ended Manually."));
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}


void UMeleeAbility::HitTarget(const FHitResult& HitResult)
{
	OnTargetHit(HitResult);
	if (HitEffect)
	{
		const FGameplayEffectSpecHandle HitEffectSpec = MakeOutgoingGameplayEffectSpec(HitEffect);
		FGameplayAbilityTargetDataHandle TargetDataHandle;
		TargetDataHandle.Add(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));
		
		ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
			HitEffectSpec, TargetDataHandle);
	}
}