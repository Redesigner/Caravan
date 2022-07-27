// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetingReticleActor.generated.h"

UENUM()
enum EReticleMovementMode
{
	FollowOnwer,
	FollowInput,
	Static
};

UCLASS()
class CARAVANABILITY_API ATargetingReticleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATargetingReticleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION( BlueprintImplementableEvent )
	void OnFadeIn();

	UFUNCTION( BlueprintImplementableEvent )
	void OnFadeOut();

	UFUNCTION( BlueprintCallable )
	FVector GetGroundLocation();

	UFUNCTION( NetMulticast, Unreliable )
	void MulticastUpdateLocation(FVector Location);

	UFUNCTION( Server, Unreliable )
	void ClientSetLocation(FVector Location);

	UFUNCTION( NetMulticast, Unreliable, BlueprintCallable )
	void MulticastSetReticleMovementMode(EReticleMovementMode NewMovementMode);

	void SetController(AController* NewController);

	void FadeIn();

	void FadeOut();

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MovementSpeed = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EReticleMovementMode> MovementMode = FollowOnwer;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bGet = true;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Ownership, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AController> Controller;
};
