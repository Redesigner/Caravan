// Copyright (c) 2023 Stephen Melnick


#include "CaravanAbility/Sequencing/AnimNotify_SetComboState.h"

#include "CaravanGame/Character/CharacterBase.h"

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
		// CharacterBase->SetComboState(ComboState);
	}
}
