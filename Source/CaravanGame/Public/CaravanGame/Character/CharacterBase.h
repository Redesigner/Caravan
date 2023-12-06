// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h" 
#include "CharacterBaseAttributeSet.h"
#include "GameplayCueInterface.h"

#include "Components/PrimitiveComponent.h"

#include "CaravanGame/Dialog/DialogResponseInterface.h"
#include "CaravanGame/Dialog/Tags/DialogTagHandle.h"

#include "CaravanAbility/GameplayAbilities/Abilities/CaravanGameplayAbility.h"
#include "CaravanAbility/GameplayAbilities/Components/HitboxController.h"
#include "CaravanAbility/GameplayAbilities/Actors/TargetingReticleActor.h"

#include "CharacterBase.generated.h"

class USphereComponent;
class UCharacterAbilitySystemComponent;
class USpringArmComponent;
class UCameraComponent;


UCLASS()
class CARAVANGAME_API ACharacterBase : public ACharacter,
	public IAbilitySystemInterface, public IGameplayCueInterface,
	public IDialogResponseInterface
{
	GENERATED_BODY()
	
	// ============================== MEMBER COMPONENTS ==========================================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Targeting, meta = (AllowPrivateAccess = "true"), Replicated)
	ATargetingReticleActor* TargetingReticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	UCharacterAbilitySystemComponent* AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hitboxes, meta = (AllowPrivateAccess = "true"))
	UHitboxController* HitboxController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes, meta = (AllowPrivateAccess = "true"))
	UCharacterBaseAttributeSet* AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes, meta = (AllowPrivateAccess = "true"))
	UPrimitiveComponent* InteractionVolume;
	// ============================== END MEMBER COMPONENTS =======================================


public:
	ACharacterBase(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	/// Bind inputs for movement and GameplayAbilities
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;



	UFUNCTION(BlueprintCallable)
	UCharacterBaseAttributeSet* GetAttributeSetBase() const;

	// Initialize the GameplayAbilities from the DefaultAbilities Array
	void GrantAbilities();


	// ============================== Dialog Response Interface ==============================
	virtual const TArray<FString>& GetResponses() const override;

	virtual FDialogTagHandle* GetRootTag() override;
	// ============================== End interface ==========================================

	/**
	 * @brief Start the targeting reticle's fade in
	*/
	void ShowTargetingReticle();

	/**
	 * @brief Start the targeting reticle's fade out
	*/
	void HideTargetingReticle();

	/**
	 * @brief Get the current location of the targeting reticle
	 * @return The location of the ground below the targeting reticle, or the character's location of the targeting reticle is not valid
	*/
	FVector GetTargetingReticleLocation() const;



	// ============================== Movement functions and UFUNCTION bindings. ==============================
	// UFUNCTION bindings are necessary for these abilities to be called by the default AI controller
	UFUNCTION()
	void MoveForward(float Scale);
	UFUNCTION()
	void MoveRight(float Scale);

	UFUNCTION()
	void LookUp(float Scale);
	UFUNCTION()
	void LookRight(float Scale);
	// ============================== End movement UFUNCTIONS ==============================

	/**
	 * @brief Disable the player's movement
	*/
	void PauseMovement();
	/**
	 * @brief Enable the player's movement
	*/
	void UnpauseMovement();

	/**
	 * @brief Interact with any actors in the character's interaction volume
	*/
	void Interact();

	/**
	 * @brief Pause the character's movement and call the blueprint function
	*/
	void DialogStart();

	/**
	 * @brief Unpause the character's movement and call the blueprint function
	*/
	void DialogEnd();

	// Called after the internal dialog handling is done.
	// You should apply/remove necessary GameplayEffects here
	UFUNCTION(BlueprintImplementableEvent, meta = (BlueprintProtected))
	void OnDialogStart();

	UFUNCTION(BlueprintImplementableEvent, meta = (BlueprintProtected))
	void OnDialogEnd();

	/**
	 * @brief Add a dialog response to our response map
	 * @param Received 
	 * @param DialogId 
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	void AddDialogResponse(FName Received, FName DialogId);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath(const FGameplayEffectSpec CauseOfDeath);



	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Dialog)
	TArray<FString> DialogResponses;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Dialog)
	FString NPCDialogId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Dialog)
	FDialogTagHandle RootTag;

private:

	/**
	 * @brief Spawn a targeting reticle actor
	*/
	void SpawnTargetingReticle();
	
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

	TWeakObjectPtr<class UDialogHandler> DialogHandler;
	TWeakObjectPtr<class UInventoryContainer> InventoryContainer;
};
