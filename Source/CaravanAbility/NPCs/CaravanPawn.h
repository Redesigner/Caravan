// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AttributeSet.h"

#include "CaravanPawn.generated.h"

UCLASS()
class CARAVANABILITY_API ACaravanPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes, meta = (AllowPrivateAccess = "true"))
	UAttributeSet* AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes, meta = (AllowPrivateAccess = "true"))
	class UCharacterAbilitySystemComponent* AbilitySystem;

public:
	ACaravanPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
