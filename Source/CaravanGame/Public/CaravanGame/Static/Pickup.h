// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CaravanGame/Static/InteractableInterface.h"

#include "Pickup.generated.h"

UCLASS()
class CARAVANGAME_API APickup : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual FGameplayInteraction HandleInteraction(const FGameplayInteraction& Interaction) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPickup();

};
