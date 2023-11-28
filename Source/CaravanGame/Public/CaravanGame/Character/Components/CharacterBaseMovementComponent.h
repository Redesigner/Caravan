// Copyright (c) 2023 Stephen Melnick

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
class CARAVANGAME_API UCharacterBaseMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	UCharacterBaseMovementComponent();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;


public:
	virtual float GetMaxSpeed() const override;

	virtual FRotator GetDeltaRotation(float DeltaTime) const override;

	virtual void PhysicsRotation(float DeltaTime) override;

	/**
	 * @brief Disables character movment in a network-replication friendly way
	 * @param Time How long, in seconds, to stun the character for
	*/
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
