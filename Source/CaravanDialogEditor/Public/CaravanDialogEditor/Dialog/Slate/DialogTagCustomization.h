// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "PropertyEditor/Public/IPropertyTypeCustomization.h"

/**
 * 
 */
class ITableRow;
struct FDialogTag;

class CARAVANDIALOGEDITOR_API FDialogTagCustomization : public IPropertyTypeCustomization
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
