// Fill out your copyright notice in the Description page of Project Settings.


#include "TestMelee_GA.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_NetworkSyncPoint.h"
#include "CaravanAbility/Character/CharacterBase.h"

UTestMelee_GA::UTestMelee_GA()
{
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnlyTermination;
}

void UTestMelee_GA::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!MeleeAnimation)
	{
		return;
	}
	ApplyCooldown(Handle, ActorInfo, ActivationInfo);

	if (DurationEffect)
	{
		FGameplayEffectSpecHandle LockEffect = MakeOutgoingGameplayEffectSpec(DurationEffect);
		LockEffect.Data.Get()->SetDuration(MeleeAnimation->GetPlayLength(), true);
		
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, LockEffect);
	}
	
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
		TEXT("AnimationMontageTask"), MeleeAnimation, 1.0f);
	if (UActorComponent* ActorComponent = ActorInfo->AvatarActor->GetComponentByClass(UHitboxController::StaticClass()))
	{
		// No check required for cast, unless type reflection has an error
		UHitboxController* HitboxController = Cast<UHitboxController>(ActorComponent);
		HitboxController->HitDetectedDelegate.BindUObject(this, &UTestMelee_GA::HitTarget);
		Task->OnCompleted.AddDynamic(this, &UTestMelee_GA::HandleMontageEnd);
		Task->OnCancelled.AddDynamic(this, &UTestMelee_GA::HandleMontageEnd);
	}
	Task->ReadyForActivation();
}

void UTestMelee_GA::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{	
	const FDateTime CurrentTime = FDateTime::Now();
	int TimeMS = CurrentTime.GetMillisecond();

	if (HasAuthority(&ActivationInfo))
	{
		UE_LOG(LogAbilitySystem, Display, TEXT("Server copy of ability ended at %s : %i"), *CurrentTime.ToString(), TimeMS);
	}
	else
	{
		UE_LOG(LogAbilitySystem, Display, TEXT("Client copy of ability ended at %s : %i"), *CurrentTime.ToString(), TimeMS);
	}
	ApplyCancelEffect(Handle, ActorInfo, ActivationInfo);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTestMelee_GA::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
}

void UTestMelee_GA::HandleMontageEnd()
{
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	UHitboxController* HitboxController = AvatarActor ? AvatarActor->FindComponentByClass<UHitboxController>() : nullptr;
	if (HitboxController != nullptr)
	{
		HitboxController->ClearOverlaps();
	}

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UTestMelee_GA::HitTarget(const FHitResult& HitResult)
{
	OnTargetHit(HitResult);
	if (HitEffect)
	{
		const FGameplayEffectSpecHandle HitEffectSpec = MakeOutgoingGameplayEffectSpec(HitEffect);
		FGameplayAbilityTargetDataHandle TargetDataHandle;
		TargetDataHandle.Add(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));
		
		// ReSharper disable once CppExpressionWithoutSideEffects
		ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
			HitEffectSpec, TargetDataHandle);
	}
}

void UTestMelee_GA::ApplyCancelEffect(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	FGameplayEffectSpecHandle EffectSpec = MakeOutgoingGameplayEffectSpec(DurationCancelEffect);
	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetCurrentActorInfo()->OwnerActor))
	{
		CharacterBase->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data, ActivationInfo.GetActivationPredictionKey());
	}
}

