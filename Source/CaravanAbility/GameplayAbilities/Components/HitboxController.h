// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "HitboxController.generated.h"

// This class does not need to be modified.
UCLASS()
class UHitboxController : public USceneComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	TWeakObjectPtr<USphereComponent> GetHitboxByName(FName Name);

	TWeakObjectPtr<USphereComponent> SpawnHitbox(FName Name, FVector HitboxRelativeLocation, float Radius);

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

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = Collision, Transient)
	TArray<AActor*> QueuedHitTargets;
};
