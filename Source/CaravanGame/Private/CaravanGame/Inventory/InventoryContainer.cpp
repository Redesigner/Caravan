// Copyright (c) 2023 Stephen Melnick


#include "CaravanGame/Inventory/InventoryContainer.h"

UInventoryContainer::UInventoryContainer()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventoryContainer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

const FInventoryEntry& UInventoryContainer::GetInventoryEntry(int Index) const
{
	if (InventoryStorage.IsValidIndex(Index))
	{
		return InventoryStorage[Index];
	}
	// Temp only
	return InventoryStorage[0];
}

bool UInventoryContainer::GiveItem(const FInventoryEntry& Item)
{
	if (FirstEmpty == 0)
	{
		InventoryStorage.Add(Item);
		return true;
	}
	return true;

}

const TArray<FInventoryEntry>& UInventoryContainer::GetEntries() const
{
	return InventoryStorage;
}

