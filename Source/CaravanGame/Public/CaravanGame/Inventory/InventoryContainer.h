// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CaravanGame/Inventory/InventoryEntry.h"

#include "InventoryContainer.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CARAVANGAME_API UInventoryContainer : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryContainer();

protected:
	virtual void BeginPlay() override;

public:

	const FInventoryEntry& GetInventoryEntry(int Index) const;

	// Returns false if the item couldn't go into the inventory for whatever reason
	bool GiveItem(const FInventoryEntry& Item);

	UFUNCTION(BlueprintCallable)
	const TArray<FInventoryEntry>& GetEntries() const;

	// The internal list of all of our inventory items
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, DisplayName = Items)
	TArray<FInventoryEntry> InventoryStorage;

private:
	// The first empty slot in the inventory... it is 0 if there are no empty slots
	// i.e., the inventory is full. Since it's currently a resizable array, we can just add a new entry to the end.
	uint8 FirstEmpty = 0;
};
