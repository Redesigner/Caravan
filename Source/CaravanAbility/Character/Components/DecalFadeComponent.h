// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DecalComponent.h"
#include "DecalFadeComponent.generated.h"

/**
 * @brief Custom wrapper around UDecalComponent to add fade/in out functionality
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

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal|Material", meta = (AllowPrivateAccess = true))
	UMaterialInterface* BaseMaterial;

	UPROPERTY(Transient);
	UMaterialInstanceDynamic* InternalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal|Fade", meta = (AllowPrivateAccess = true))
	float FadeTime = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Decal|Fade", meta = (AllowPrivateAccess = true))
	float CurrentFadeTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal|Fade", meta = (AllowPrivateAccess = true))
	bool bDesiredVisiblity = false;

	bool bIsFading = false;
};
