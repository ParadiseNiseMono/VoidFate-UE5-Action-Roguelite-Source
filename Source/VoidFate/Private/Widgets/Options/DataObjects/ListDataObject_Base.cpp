// Paradise NiseMono All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

#include "VoidFateSettings/VFGameUserSettings.h"

void UListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
}

void UListDataObject_Base::AddEditCondition(const FOptionsDataEditConditionDescriptor& InEditCondition)
{
	EditConditionDescArray.Add(InEditCondition);
}

void UListDataObject_Base::AddEditDependencyData(UListDataObject_Base* InDependencyData)
{
	if (!InDependencyData->OnListDataModified.IsBoundToObject(this))
	{
		InDependencyData->OnListDataModified.AddUObject(this, &ThisClass::OnEditDependencyDataModified);
	}
}

bool UListDataObject_Base::IsDataCurrentEditable()
{
	bool bIsEditable = true;

	if (EditConditionDescArray.IsEmpty()) return bIsEditable;

	FString CachedDisabledRichReason;

	for (const FOptionsDataEditConditionDescriptor& InEditCondition : EditConditionDescArray)
	{
		if (!InEditCondition.IsValid() || InEditCondition.IsEditConditionMet()) continue;

		bIsEditable = false;
		CachedDisabledRichReason.Append(InEditCondition.GetDisabledRichReason());

		SetDisabledRichText(FText::FromString(CachedDisabledRichReason));

		if (InEditCondition.HasForcedStringValue())
		{
			const FString& InForcedValue = InEditCondition.GetDisabledForcedStringValue();

			//If the current value this data has can be set to the forced value
			if (CanSetToForcedStringValue(InForcedValue))
			{
				OnSetToForcedStringValue(InForcedValue);
			}
		}
	}
	return bIsEditable;
}

void UListDataObject_Base::OnDataObjectInitialized()
{
}

void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* ModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	OnListDataModified.Broadcast(ModifiedData, ModifyReason);

	if (bShouldApplyChangeImmediately)
	{
		UVFGameUserSettings::Get()->ApplySettings(true);
	}
}

void UListDataObject_Base::OnEditDependencyDataModified(UListDataObject_Base* ModifiedDependencyData,
	EOptionsListDataModifyReason ModifyReason)
{
	OnDependencyDataModified.Broadcast(ModifiedDependencyData, ModifyReason);
}
