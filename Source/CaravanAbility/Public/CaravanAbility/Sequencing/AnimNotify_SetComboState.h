// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SetComboState.generated.h"

/**
 * 
 */
UCLASS()
class CARAVANABILITY_API UAnimNotify_SetComboState : public UAnimNotify
{
	GENERATED_BODY()

		UAnimNotify_SetComboState();

public:

	virtual bool ShouldFireInEditor() override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ComboState;
	
};
