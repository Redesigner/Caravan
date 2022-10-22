// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "ArmorComponent.generated.h"

/**
 * 
 */
UCLASS()
class CARAVANABILITY_API UArmorComponent : public USphereComponent
{
	GENERATED_BODY()

	UArmorComponent(const FObjectInitializer& ObjectInitializer);

public:
	bool CanBlockHit(const struct FGameplayAbilitySpec* HitSpec) const;
	
	TWeakObjectPtr<class UHitboxController> OwningHitboxController;
};
