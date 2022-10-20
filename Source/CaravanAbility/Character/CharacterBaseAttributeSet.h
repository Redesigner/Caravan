// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "CharacterBaseAttributeSet.generated.h"

/**
 * 
 */
struct FGameplayEffectSpec;

UCLASS()
class CARAVANABILITY_API UCharacterBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	UCharacterBaseAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

public:

	UFUNCTION()
	virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Movement)
	FGameplayAttributeData MovementSpeed;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = Health, ReplicatedUsing = OnRep_CurrentHealth)
	FGameplayAttributeData CurrentHealth;

	DECLARE_DELEGATE_OneParam(FOnDeath, const FGameplayEffectSpec)
	FOnDeath OnDeath;
};
