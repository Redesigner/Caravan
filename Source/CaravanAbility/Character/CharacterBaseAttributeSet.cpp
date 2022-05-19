// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBaseAttributeSet.h"

UCharacterBaseAttributeSet::UCharacterBaseAttributeSet()
{
	MovementSpeed.SetBaseValue(600.0f);
	MovementSpeed.SetCurrentValue(600.0f);

	CurrentHealth.SetBaseValue(100.0f);
	CurrentHealth.SetCurrentValue(100.0f);
}

