// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogTagCustomization.h"

#include "PropertyEditor/Public/DetailLayoutBuilder.h"
#include "PropertyEditor/Public/DetailWidgetRow.h"
#include "PropertyEditor/Public/PropertyHandle.h"

#include "SlateBasics.h"

#include "IDetailChildrenBuilder.h"

#include "CaravanAbility\Dialog\DialogSubsystem.h"
#include "CaravanAbility\Dialog\Tags\DialogTag.h"
#include "CaravanAbility\Dialog\Tags\DialogTagHandle.h"
#include "CaravanAbility\Dialog\Tags\DialogTagManager.h"

#include "CaravanAbilityDialogEditor\Dialog\Slate\DialogTagViewer.h"

#include "Widgets/Input/SComboButton.h" 
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Views/STreeView.h" 
#include "Widgets/Views/STableRow.h"

#define LOCTEXT_NAMESPACE "DialogEditor"

TSharedRef<IPropertyTypeCustomization> FDialogTagCustomization::MakeInstance()
{
	return MakeShareable(new FDialogTagCustomization);
}

void FDialogTagCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	void* ValueData;
	StructPropertyHandle->GetValueData(ValueData);
	FDialogTagHandle* DialogTagHandle = reinterpret_cast<FDialogTagHandle*>(ValueData);
	const FDialogTag* DialogTag = DialogTagHandle->GetTag();
	check(GEngine);
	UDialogSubsystem* DialogSubsystem = GEngine->GetEngineSubsystem<UDialogSubsystem>();
	check(DialogSubsystem);

	FString Name = TEXT("Invalid");
	if (DialogTag)
	{
		Name = DialogTagHandle->GetFullName();
	}

	PropertyDisplayText = FText::FromString(Name);

	TagViewer = SNew(SDialogTagViewer)
		.PropertyHandle(StructPropertyHandle)
		.DialogTagManager(DialogSubsystem->GetTagManager());
	HeaderRow.NameContent()[StructPropertyHandle->CreatePropertyNameWidget()]
		.ValueContent()[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				[
					TagViewer.ToSharedRef()
				]];
	TagViewer->SetDisplayText(PropertyDisplayText);

	OnPropertyChanged(StructPropertyHandle);
	const TSharedPtr<IPropertyHandle> PropertyHandle = StructPropertyHandle;
	const FSimpleDelegate PropertyChangedDelegate = FSimpleDelegate::CreateSP(this, &FDialogTagCustomization::OnPropertyChanged, PropertyHandle);
	
	StructPropertyHandle->SetOnPropertyValueChanged(PropertyChangedDelegate);
}

void FDialogTagCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
}

void FDialogTagCustomization::OnPropertyChanged(TSharedPtr<IPropertyHandle> PropertyHandle)
{
	if (PropertyHandle.IsValid() && PropertyHandle->IsValidHandle())
	{
		void* ValueData;
		PropertyHandle->GetValueData(ValueData);
		FDialogTagHandle* DialogTagHandle = reinterpret_cast<FDialogTagHandle*>(ValueData);
		PropertyDisplayText = FText::FromString(DialogTagHandle->GetFullName());
		TagViewer->SetDisplayText(PropertyDisplayText);
	}
}

#undef LOCTEXT_NAMESPACE