// Copyright (c) 2023 Stephen Melnick


#include "CaravanAbility/Sequencing/AnimNotify_ClearHitboxes.h"

#include "CaravanAbility/GameplayAbilities/Components/HitboxController.h"

UAnimNotify_ClearHitboxes::UAnimNotify_ClearHitboxes()
{
	bShouldFireInEditor = false;
}


bool UAnimNotify_ClearHitboxes::ShouldFireInEditor()
{
	return false;
}


void UAnimNotify_ClearHitboxes::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp->GetOwner())
	{
		return;
	}
	UActorComponent* HitboxControllerComponent = MeshComp->GetOwner()->GetComponentByClass(UHitboxController::StaticClass());
	if (UHitboxController* HitboxController = Cast<UHitboxController>(HitboxControllerComponent))
	{
		HitboxController->ClearOverlaps();
	}
}
