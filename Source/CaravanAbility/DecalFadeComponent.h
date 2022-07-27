// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DecalComponent.h"
#include "DecalFadeComponent.generated.h"

/**
 * 
 */
UCLASS()
class CARAVANABILITY_API UDecalFadeComponent : public UDecalComponent
{
	GENERATED_BODY()

	UDecalFadeComponent();

	virtual void OnVisibilityChanged() override;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal|Material")
	UMaterialInterface* BaseMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal|Fade")
	float FadeTime = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Decal|Fade")
	float CurrentFadeTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal|Fade")
	bool bDesiredVisiblity = false;

private:
	UPROPERTY(Transient);
	UMaterialInstanceDynamic* InternalMaterial;

	bool bIsFading = false;
};
