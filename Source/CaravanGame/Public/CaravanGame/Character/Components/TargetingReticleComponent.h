// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TargetingReticleComponent.generated.h"

/**
 * @brief A component that represents a player's targeting reticle in the world
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CARAVANGAME_API UTargetingReticleComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UTargetingReticleComponent();

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/**
	 * @brief Get the location of the ground below the targeting reticle
	 * @param MaxDistance Distance below the targeting reticle's location we check for the ground, if it is floating
	 * @return Where the raycast landed, or the targeting reticle's location if we could not find the ground
	*/
	FVector GetGroundLocation(float MaxDistance = 250.0f) const;
		
};
