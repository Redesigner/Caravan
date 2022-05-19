// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Hitbox.h"

#include "CaravanAbility/Character/CharacterBase.h"

void UAnimNotify_Hitbox::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	UActorComponent* HitboxControllerComponent = MeshComp->GetOwner()->GetComponentByClass(UHitboxController::StaticClass());
	if (UHitboxController* HitboxController = Cast<UHitboxController>(HitboxControllerComponent))
	{
		HitboxController->SpawnHitbox(HitboxName, HitboxRelativeLocation, HitboxRadius);
	}
}


void UAnimNotify_Hitbox::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	UActorComponent* HitboxControllerComponent = MeshComp->GetOwner()->GetComponentByClass(UHitboxController::StaticClass());
	if (UHitboxController* HitboxController = Cast<UHitboxController>(HitboxControllerComponent))
	{
		HitboxController->RemoveHitboxByName(HitboxName);
	}
}
