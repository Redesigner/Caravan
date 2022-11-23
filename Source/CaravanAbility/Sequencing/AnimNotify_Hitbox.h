// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotify_Hitbox.generated.h"

/**
 * 
 */
UCLASS()
class CARAVANABILITY_API UAnimNotify_Hitbox : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;



	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	FName HitboxName = TEXT("New hitbox");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MakeEditWidget, AllowPrivateAccess = true))
	FVector HitboxRelativeLocation = FVector::Zero();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	bool bHitboxHasDirection = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MakeEditWidget, EditCondition = "bHitboxHasDirection", AllowPrivateAccess = true))
	FVector HitboxDirection = FVector::Zero();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	float HitboxRadius;

public:
	UPROPERTY()
	class USphereComponent* VisualizerSphere;
};
