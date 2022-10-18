// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDialogEditor, Log, All);

class FCaravanAbilityDialogEditorModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	void* CustomRejectionTest = nullptr;
};
