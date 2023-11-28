// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ClearHitboxes.generated.h"

/**
 * This notification needs to be branching, if used in multiplayer. I can potentially implement accumulating states, but for now, it can skip
 */
UCLASS()
class CARAVANABILITY_API UAnimNotify_ClearHitboxes : public UAnimNotify
{
	GENERATED_BODY()

	UAnimNotify_ClearHitboxes();

public:

	virtual bool ShouldFireInEditor() override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
