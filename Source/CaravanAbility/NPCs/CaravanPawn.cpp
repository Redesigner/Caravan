// Fill out your copyright notice in the Description page of Project Settings.


#include "CaravanPawn.h"

#include "CaravanAbility/GameplayAbilities/Components/CharacterAbilitySystemComponent.h"
#include "AttributeSet.h"

// Sets default values
ACaravanPawn::ACaravanPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystem = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UAttributeSet>(TEXT("Character attribute set"));
}

// Called when the game starts or when spawned
void ACaravanPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACaravanPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACaravanPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

