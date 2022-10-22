// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorComponent.h"

UArmorComponent::UArmorComponent(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
    ShapeColor = FColor::Blue;
}

bool UArmorComponent::CanBlockHit(const FGameplayAbilitySpec* HitSpec) const
{
    // @TODO: Compare levels of armor and attack correctly... for now all attacks will be blocked
    return true;
}
