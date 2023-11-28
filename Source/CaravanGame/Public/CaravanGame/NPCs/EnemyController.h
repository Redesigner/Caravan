// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameplayTagContainer.h" 
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class CARAVANGAME_API AEnemyController : public AAIController
{
	GENERATED_BODY()

	AEnemyController();

public:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* Pawn) override;


	void TargetSpawned(AActor* Target);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsValidTarget(AActor* Target) const;

	UFUNCTION()
	void EndInternalCooldown();

	void TryExecuteAnyAttack();

	UFUNCTION()
	void HandleComboStateChanged(const struct FComboState& NewComboState);

protected:
	TArray<TWeakObjectPtr<AActor>> GetTargets() const;
	virtual bool IsTarget(AActor* Target) const;

private:
	TWeakObjectPtr<AActor> GetClosestTarget() const;
	TArray<TWeakObjectPtr<AActor>> PotentialTargets;

	UPROPERTY(VisibleInstanceOnly, Category = Targeting, meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<AActor> CurrentTarget;

	UPROPERTY(EditAnywhere, Category = Attacking, meta = (AllowPrivateAccess = true))
	FGameplayTagContainer ValidAttackTagContainer;

	// ===== References ======
	// Weak member references, just to avoid recasting every time. This means this controller won't technically work with other
	// component and actor types, but this is fine *for now*
	// Try to make these forward declared where possible!
	TWeakObjectPtr<class ACharacterBase> Character;

	TWeakObjectPtr<class UCharacterMovementComponent> MovementComponent;

	TWeakObjectPtr<class UCharacterAbilitySystemComponent> CharacterAsc;
	// ====== End weak references =====
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true), Category = Attacking)
	float MinAttackDistance = 150.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true), Category = Attacking)
	float AcceptableConeAngle = 35.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true), Category = Attacking)
	float SuccessfulAttackCooldown = 2.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true), Category = Targeting)
	float MaxViewDistance = 250.0f;

	FTimerHandle InternalAttackCooldown;

	bool bInternalCooldown = false;
};
