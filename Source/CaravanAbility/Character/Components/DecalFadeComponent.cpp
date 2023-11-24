// Fill out your copyright notice in the Description page of Project Settings.


#include "DecalFadeComponent.h"

UDecalFadeComponent::UDecalFadeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDecalFadeComponent::OnVisibilityChanged()
{
	bDesiredVisiblity = GetVisibleFlag();
	bIsFading = true;
	SetVisibleFlag(CurrentFadeTime < FadeTime);
	Super::OnVisibilityChanged();
}


void UDecalFadeComponent::BeginPlay()
{
	SetVisibility(false);
	CurrentFadeTime = bDesiredVisiblity ? 0.0f : FadeTime;
	if (BaseMaterial)
	{
		InternalMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, NULL);
		DecalMaterial = InternalMaterial;
	}
}

void UDecalFadeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bIsFading)
	{
		CurrentFadeTime += DeltaTime * (bDesiredVisiblity ? -1.0f : 1.0f);
		if (CurrentFadeTime < 0.0f)
		{
			CurrentFadeTime = 0.0f;
			bIsFading = false;
		}
		else if (CurrentFadeTime > FadeTime)
		{
			CurrentFadeTime = FadeTime;
			bIsFading = false;
		}
		SetVisibility(CurrentFadeTime < FadeTime);
	}
	if (InternalMaterial)
	{
		InternalMaterial->SetScalarParameterValue(FName("Opacity"), 1.0f - CurrentFadeTime / FadeTime);
	}
}

void UDecalFadeComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property == this->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UDecalFadeComponent, BaseMaterial)) )
	{
		if (BaseMaterial)
		{
			InternalMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, NULL);
			InternalMaterial->SetScalarParameterValue(FName("Opacity"), 1.0f - CurrentFadeTime / FadeTime);
			DecalMaterial = InternalMaterial;
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
