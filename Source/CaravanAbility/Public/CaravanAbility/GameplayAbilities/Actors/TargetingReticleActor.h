// Copyright (c) 2023 Stephen Melnick

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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void PostNetReceiveLocationAndRotation() override;


	UFUNCTION( BlueprintImplementableEvent )
	void OnFadeIn();

	UFUNCTION( BlueprintImplementableEvent )
	void OnFadeOut();

	UFUNCTION( BlueprintCallable )
	FVector GetGroundLocation();

	UFUNCTION(Server, Reliable)
	void ServerSetLocation(FVector Location);

	UFUNCTION( NetMulticast, Reliable )
	void FadeIn();

	UFUNCTION(NetMulticast, Reliable)
	void FadeOut();

	void SetOwningPawn(APawn* NewPawn);

	UFUNCTION()
	void OnRep_SetOwningPawn();

private:
	FVector GetSmoothedLocation(const float DeltaTime) const;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MovementSpeed = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement|Replication", meta = (AllowPrivateAccess = "true"))
	float MinDelay = 0.1f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement|Replication", meta = (AllowPrivateAccess = "true"))
	float MaxDelay = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"), Replicated)
	TEnumAsByte<EReticleMovementMode> MovementMode = FollowOnwer;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = OnRep_SetOwningPawn)
	APawn* OwningPawn;

	// Client only
	FVector DesiredLocation;
};
