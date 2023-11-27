// Copyright 2022 Stephen Melnick


#include "DefendAbility.h"

#include "AbilitySystemComponent.h"

#include "CaravanAbility/GameplayAbilities/Components/CharacterAbilitySystemComponent.h"
#include "CaravanAbility\GameplayAbilities\Components\HitboxController.h"
#include "CaravanAbility\Character\CharacterBase.h"

void UDefendAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

void UDefendAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (BlockingEffect)
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(BlockingEffect);
		EffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
	if (UActorComponent* Component = ActorInfo->OwnerActor->GetComponentByClass(UHitboxController::StaticClass()))
	{
		for (int i = 0; i < ArmorInstances.Num(); i++)
		{
			HitboxController = Cast<UHitboxController>(Component);
			HitboxController->SpawnArmor(TEXT("Armor") + i, ArmorInstances[i].Position, ArmorInstances[i].Rotation,
				ArmorInstances[i].Normal, ArmorInstances[i].Dimensions, ArmorInstances[i].Shape);
		}
	}

	if (ACharacterBase* Character = Cast<ACharacterBase>(GetOwningActorFromActorInfo()))
	{
		Character->PauseMovement();
	}
	if (HitboxController.IsValid())
	{
		if (HasAuthority(&ActivationInfo))
		{
			HitboxController->HitBlockedDelegate.BindUObject(this, &UDefendAbility::BlockHit);
		}
		else
		{
			HitboxController->HitBlockedDelegate.BindUObject(this, &UDefendAbility::BlockHitLocal);
		}
	}
}


void UDefendAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(EffectHandle);

	if (HitboxController.IsValid())
	{
		for (int i = 0; i < ArmorInstances.Num(); i++)
		{
			HitboxController->RemoveArmorByName(TEXT("Armor") + i);
		}
		HitboxController->HitBlockedDelegate.Unbind();
	}

	if (EffectHandle.IsValid())
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		ASC->RemoveActiveGameplayEffect(EffectHandle);
	}
	if (ACharacterBase* Character = Cast<ACharacterBase>(GetOwningActorFromActorInfo()))
	{
		Character->UnpauseMovement();
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDefendAbility::BlockHit(const FHitResult& HitResult)
{
	OnHitBlocked(HitResult);
	ApplyGameplayCue(HitResult);
	// Additional server-side logic, like self stuns, here
}

void UDefendAbility::BlockHitLocal(const FHitResult& HitResult)
{
	OnHitBlocked(HitResult);
	ApplyGameplayCue(HitResult);
}

void UDefendAbility::ApplyGameplayCue(const FHitResult& HitResult)
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
