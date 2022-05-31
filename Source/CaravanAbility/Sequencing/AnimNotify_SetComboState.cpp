// Fill out your copyright notice in the Description page of Project Settings.
#include "AnimNotify_SetComboState.h"

#include "CaravanAbility/Character/CharacterBase.h"

UAnimNotify_SetComboState::UAnimNotify_SetComboState()
{
	bShouldFireInEditor = false;
}

bool UAnimNotify_SetComboState::ShouldFireInEditor()
{
	return false;
}

void UAnimNotify_SetComboState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp->GetOwner();
	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(Owner))
	{
		CharacterBase->SetComboState(ComboState);
	}
}
