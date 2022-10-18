// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"

#include "DialogSubsystem.generated.h"

/**
 * 
 */
struct FDialogTagHandle;
class UDialogTagManager;
struct FDialogTag;

UCLASS()
class CARAVANABILITY_API UDialogSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:

	UDialogSubsystem(const FObjectInitializer& ObjectInitializer);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UDialogTagManager* GetTagManager();

private:
	UPROPERTY(Transient)
	UDialogTagManager* DialogTagManager;
};
