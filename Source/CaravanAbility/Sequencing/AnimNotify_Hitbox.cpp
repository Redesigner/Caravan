// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Hitbox.h"

#include "CaravanAbility/Character/CharacterBase.h"

void UAnimNotify_Hitbox::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (!MeshComp->GetOwner())
	{
		return;
	}
	UActorComponent* HitboxControllerComponent = MeshComp->GetOwner()->GetComponentByClass(UHitboxController::StaticClass());
	if (UHitboxController* HitboxController = Cast<UHitboxController>(HitboxControllerComponent))
	{
		// We need to somehow get the current ability from this context...
		HitboxController->SpawnHitbox(HitboxName, HitboxRelativeLocation, HitboxDirection, HitboxRadius);
	}
}


void UAnimNotify_Hitbox::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp->GetOwner())
	{
		return;
	}
	UActorComponent* HitboxControllerComponent = MeshComp->GetOwner()->GetComponentByClass(UHitboxController::StaticClass());
	if (UHitboxController* HitboxController = Cast<UHitboxController>(HitboxControllerComponent))
	{
		HitboxController->RemoveHitboxByName(HitboxName);
	}
}

void UAnimNotify_Hitbox::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UAnimNotify_Hitbox, HitboxDirection))
	{
		HitboxDirection = HitboxDirection.GetSafeNormal();
	}
}
