// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyEditor/Public/IPropertyTypeCustomization.h"

/**
 * 
 */
class ITableRow;
struct FDialogTag;

class CARAVANABILITYDIALOGEDITOR_API FDialogTagCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

	void OnPropertyChanged(TSharedPtr<IPropertyHandle> PropertyHandle);

protected:

	FText PropertyDisplayText;

private:
	TSharedPtr<class SDialogTagViewer> TagViewer;

};
