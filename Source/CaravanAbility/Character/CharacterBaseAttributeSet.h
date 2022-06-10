// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "CharacterBaseAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class CARAVANABILITY_API UCharacterBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	UCharacterBaseAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	UFUNCTION()
	virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Movement)
	FGameplayAttributeData MovementSpeed;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = Health, ReplicatedUsing = OnRep_CurrentHealth)
	FGameplayAttributeData CurrentHealth;
};
