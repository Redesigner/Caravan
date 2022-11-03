// Copyright 2022 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CaravanAbility\Static\InteractableInterface.h"

#include "Pickup.generated.h"

UCLASS()
class CARAVANABILITY_API APickup : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual FGameplayInteraction HandleInteraction(const FGameplayInteraction& Interaction) override;

};
