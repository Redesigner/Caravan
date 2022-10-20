// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h" 
#include "CharacterBaseAttributeSet.h"
#include "GameplayCueInterface.h"

#include "Components/PrimitiveComponent.h"

#include "CaravanAbility/Dialog/GameplayInteraction.h"

#include "CaravanAbility/GameplayAbilities/CaravanGameplayAbility.h"
#include "CaravanAbility/GameplayAbilities/Components/HitboxController.h"
#include "CaravanAbility/GameplayAbilities/Actors/TargetingReticleActor.h"

#include "CaravanAbility/Dialog/DialogResponseInterface.h"
#include "CaravanAbility/Dialog/Tags/DialogTagHandle.h"

#include "CharacterBase.generated.h"

class USphereComponent;
class UCharacterAbilitySystemComponent;
UCLASS()
class CARAVANABILITY_API ACharacterBase : public ACharacter, public IAbilitySystemInterface, public IGameplayCueInterface, public IDialogResponseInterface
{
	GENERATED_BODY()
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Targeting, meta = (AllowPrivateAccess = "true"), Replicated)
	ATargetingReticleActor* TargetingReticle;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes, meta = (AllowPrivateAccess = "true"))
	UPrimitiveComponent* InteractionVolume;

public:
	// Sets default values for this character's properties
	ACharacterBase(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Bind inputs for movement and GameplayAbilities
	/// </summary>
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	// Dialog Response Interface
	virtual const TArray<FString>& GetResponses() const override;

	virtual FDialogTagHandle* GetRootTag() override;
	// End interface

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	UCharacterBaseAttributeSet* GetAttributeSetBase() const;
	
	void GrantAbilities();


	// Targeting reticle functions. Should these be moved somewhere else?
	void SpawnTargetingReticle();

	void ShowTargetingReticle();

	const FVector HideTargetingReticle();

	// Movement functions and UFUNCTION bindings.
	// UFUNCTION bindings are necessary for these abilities to be called by the default AI controller
	UFUNCTION()
	void MoveForward(float Scale);
	UFUNCTION()
	void MoveRight(float Scale);

	UFUNCTION()
	void LookUp(float Scale);
	UFUNCTION()
	void LookRight(float Scale);

	void PauseMovement();
	void UnpauseMovement();

	void Interact();

	void DialogStart();
	void DialogEnd();
	UFUNCTION(BlueprintCallable)
	void HandleInteraction(FGameplayInteraction Interaction);

	// Called after the internal dialog handling is done.
	// You should apply/remove necessary GameplayEffects here
	UFUNCTION(BlueprintImplementableEvent, meta = (BlueprintProtected))
	void OnDialogStart();
	UFUNCTION(BlueprintImplementableEvent, meta = (BlueprintProtected))
	void OnDialogEnd();

	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	void AddDialogResponse(FName Received, FName DialogId);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnHandleInteraction(FGameplayInteraction Interaction);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath(const FGameplayEffectSpec CauseOfDeath);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Dialog)
	TArray<FString> DialogResponses;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Dialog)
	FString NPCDialogId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Dialog)
	FDialogTagHandle RootTag;

private:
	
	UFUNCTION(Server, Reliable)
	void PauseMovementServer();
	void PauseMovementLocal();

	UFUNCTION(Server, Reliable)
	void UnpauseMovementServer();
	void UnpauseMovementLocal();

	TMap<FName, FName> ResponseMap;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Targeting, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATargetingReticleActor> TargetingReticleClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float LookSpeed = 50.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UCaravanGameplayAbility>> DefaultAbilities;
};
