// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/Object.h"
#include "CharacterBaseMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class CARAVANABILITY_API UCharacterBaseMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	UCharacterBaseMovementComponent();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;


public:
	virtual float GetMaxSpeed() const override;

	virtual FRotator GetDeltaRotation(float DeltaTime) const override;

	virtual void PhysicsRotation(float DeltaTime) override;


	void DisableMovementWithStun(float Time);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attributes")
	FGameplayAttribute MovementSpeed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, ReplicatedUsing = ReplicateStun)
	bool bIsStunned;

private:
	UFUNCTION()
	void ReplicateStun(bool bStunState);
	
	UPROPERTY()
	FTimerHandle StunTimer;
};
