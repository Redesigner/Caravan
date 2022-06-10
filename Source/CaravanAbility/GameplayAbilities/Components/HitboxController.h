// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitboxComponent.h"
#include "HitboxController.generated.h"

// This class does not need to be modified.
UCLASS()
class UHitboxController : public USceneComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	TWeakObjectPtr<UHitboxComponent> GetHitboxByName(FName Name);

	TWeakObjectPtr<UHitboxComponent> SpawnHitbox(FName Name, FVector HitboxRelativeLocation, FVector Direction, float Radius);

	void RemoveHitboxByName(FName Name);

	UFUNCTION()
	void AccumulateOverlaps(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void ClearOverlaps();


	DECLARE_DELEGATE_OneParam(FOnHitDetected, const FHitResult&);
	FOnHitDetected HitDetectedDelegate;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Collision)
	FCollisionProfileName HitboxCollisionProfile;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = Collision, Transient)
	TArray<FHitResult> QueuedHits;
};
