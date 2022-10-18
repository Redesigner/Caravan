// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogSubsystem.h"

#include "CaravanAbility\Dialog\Tags\DialogTagManager.h"


UDialogSubsystem::UDialogSubsystem(const FObjectInitializer& ObjectInitializer)
{
}

void UDialogSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	DialogTagManager = NewObject<UDialogTagManager>(this, TEXT("Dialog Tag Manager"));
	DialogTagManager->LoadFile();
	Super::Initialize(Collection);
}


void UDialogSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

UDialogTagManager* UDialogSubsystem::GetTagManager()
{
	return DialogTagManager;
}
