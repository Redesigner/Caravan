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


	virtual void ReplicateMoveToServer(float DeltaTime, const FVector& NewAcceleration) override;


public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* TickFunction) override;

	virtual float GetMaxSpeed() const override;

	virtual FRotator GetDeltaRotation(float DeltaTime) const override;


	void DisableMovementWithStun(float Time);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attributes")
	FGameplayAttribute MovementSpeed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	bool bIsStunned;

private:
	
	UPROPERTY()
	FTimerHandle StunTimer;
};
