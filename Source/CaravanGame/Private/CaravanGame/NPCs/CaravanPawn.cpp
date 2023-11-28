// Copyright (c) 2023 Stephen Melnick


#include "CaravanGame/NPCs/CaravanPawn.h"

#include "CaravanAbility/GameplayAbilities/Components/CharacterAbilitySystemComponent.h"
#include "AttributeSet.h"

ACaravanPawn::ACaravanPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystem = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UAttributeSet>(TEXT("Character attribute set"));
}

void ACaravanPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACaravanPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACaravanPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

