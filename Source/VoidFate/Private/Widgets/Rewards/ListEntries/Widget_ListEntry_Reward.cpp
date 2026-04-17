// Paradise NiseMono All Rights Reserved


#include "Widgets/Rewards/ListEntries/Widget_ListEntry_Reward.h"

#include "Widgets/Components/VFCommonButtonBase.h"
#include "Widgets/Rewards/DataObjects/ListDataObject_Reward.h"


void UWidget_ListEntry_Reward::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CommonButton_Background)
	{
		CommonButton_Background->OnClicked().AddUObject(this, &ThisClass::OnBackgroundButtonClicked);
	}
}

void UWidget_ListEntry_Reward::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);
	
	CachedOwningRewardDataObject = CastChecked<UListDataObject_Reward>(InOwningListDataObject);
}

void UWidget_ListEntry_Reward::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	Super::OnOwningListDataObjectModified(OwningModifiedData, ModifyReason);
}

void UWidget_ListEntry_Reward::OnBackgroundButtonClicked()
{
	SelectThisEntryWidget();
}
