// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CaravanAbility/GameplayAbilities/CaravanGameplayAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h" 
#include "CharacterBaseAttributeSet.h"
#include "GameplayCueInterface.h"
#include "CaravanAbility/GameplayAbilities/Components/HitboxController.h"
#include "CharacterBase.generated.h"

class USphereComponent;
class UCharacterAbilitySystemComponent;
UCLASS()
class CARAVANABILITY_API ACharacterBase : public ACharacter, public IAbilitySystemInterface, public IGameplayCueInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	UCharacterAbilitySystemComponent* AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hitboxes, meta = (AllowPrivateAccess = "true"))
	UHitboxController* HitboxController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes, meta = (AllowPrivateAccess = "true"))
	UCharacterBaseAttributeSet* AttributeSet;
	
public:
	// Sets default values for this character's properties
	ACharacterBase(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	UCharacterBaseAttributeSet* GetAttributeSetBase() const;
	
	void GrantAbilities();

	void SetComboState(FName NewComboState);
	FName GetComboState() const;

	UFUNCTION()
	void MoveForward(float Scale);
	UFUNCTION()
	void MoveRight(float Scale);

	void LookUp(float Scale);
	void LookRight(float Scale);

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float LookSpeed = 50.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UCaravanGameplayAbility>> DefaultAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	FName ComboState;
};


UENUM(BlueprintType)
enum class EMeleeInputID : uint8
{
	Attack		UMETA(DisplayName = "Attack"),
	Secondary	UMETA(DisplayName = "Secondary"),
	None		UMETA(DisplayName = "None"),
	Confirm		UMETA(DisplayName = "Confirm"),
	Cancel		UMETA(DisplayName = "Cancel")
};
