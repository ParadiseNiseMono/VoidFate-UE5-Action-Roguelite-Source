// Paradise NiseMono All Rights Reserved


#include "Widgets/Options/Widget_OptionsScreen.h"

#include "ICommonInputModule.h"
#include "VFDebugHelper.h"
#include "Input/CommonUIInputTypes.h"
#include "Subsystems/VFUISubsystem.h"
#include "VoidFateSettings/VFGameUserSettings.h"
#include "Widgets/Components/VFCommonButtonBase.h"
#include "Widgets/Components/VFCommonListView.h"
#include "Widgets/Components/VFTabListWidgetBase.h"
#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/Widget_OptionsDetailsView.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"
#include "VFFunctionLibrary.h"


void UWidget_OptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(
				ResetAction,
				true,
				FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered)
			)
		);
	}
	RegisterUIActionBinding(
		FBindUIActionArgs(
		ICommonInputModule::GetSettings().GetDefaultBackAction(),
		true,
		FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
			)
		);
	TabListWidget_OptionsTab->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnOptionsTabSelected);

	CommonListView_OptionsList->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::OnListViewItemHovered);
	CommonListView_OptionsList->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnListViewItemSelected);
}

void UWidget_OptionsScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (UListDataObject_Collection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredOptionsTabCollections())
	{
		if (!TabCollection) continue;

		const FName TabID = TabCollection->GetDataID();

		if (TabListWidget_OptionsTab->GetTabButtonBaseByID(TabID) != nullptr) continue;

		TabListWidget_OptionsTab->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());
	}
}

void UWidget_OptionsScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	UVFGameUserSettings::Get()->ApplySettings(true);
}

UWidget* UWidget_OptionsScreen::NativeGetDesiredFocusTarget() const
{
	if (UObject* SelectedObject = CommonListView_OptionsList->GetSelectedItem())
	{
		if (UUserWidget* SelectedWidget = CommonListView_OptionsList->GetEntryWidgetFromItem(SelectedObject))
		{
			return SelectedWidget;
		}
	}
	return Super::NativeGetDesiredFocusTarget();
}

UOptionsDataRegistry* UWidget_OptionsScreen::GetOrCreateDataRegistry()
{
	/*if (!CreatedOwningDataRegistry)
	{
		CreatedOwningDataRegistry = NewObject<UOptionsDataRegistry>();
		CreatedOwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
	}*/

	CreatedOwningDataRegistry = NewObject<UOptionsDataRegistry>();
	CreatedOwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());

	checkf(CreatedOwningDataRegistry, TEXT("Data Registry for options screen is not valid"));

	return CreatedOwningDataRegistry;
}

void UWidget_OptionsScreen::OnResetBoundActionTriggered()
{
	if (ResettableDataArray.IsEmpty()) return;

	UCommonButtonBase* SelectedTabButton = TabListWidget_OptionsTab->GetTabButtonBaseByID(TabListWidget_OptionsTab->GetSelectedTabId());

	const FString SelectedTabButtonName = CastChecked<UVFCommonButtonBase>(SelectedTabButton)->GetButtonDisplayName().ToString();

	UVFUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::YesNo,
		FText(UVFFunctionLibrary::GetOptionsScreenTitleText("ResetKey")),
		FText::FromString(UVFFunctionLibrary::GetOptionsScreenTitleText("ResetDescKey").ToString() + SelectedTabButtonName + UVFFunctionLibrary::GetOptionsScreenTitleText("TabKey").ToString()),
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			if (ClickedButtonType != EConfirmScreenButtonType::Confirmed) return;

			bool bHasDataFailedToReset = false;
			bIsResettingData = true;
			
			for (UListDataObject_Base* DataToReset : ResettableDataArray)
			{
				if (!DataToReset) continue;

				if (DataToReset->TryResetBackToDefaultValue())
				{
					/*Debug::Print(DataToReset->GetDataDisplayName().ToString() + TEXT(" reset"));*/
					UVFUISubsystem::Get(this)->OnKeybindingChanged.Broadcast();
				}
				else
				{
					bHasDataFailedToReset = true;
					Debug::Print(DataToReset->GetDataDisplayName().ToString() + TEXT(" failed to reset"));
				}
			}
			
			if (!bHasDataFailedToReset)
			{
				ResettableDataArray.Empty();
				RemoveActionBinding(ResetActionHandle);
			}
			bIsResettingData = false;
		}
	);
}

void UWidget_OptionsScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}

void UWidget_OptionsScreen::OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered)
{
	if (!InHoveredItem) return;
	
	UWidget_ListEntry_Base* HoveredEntryWidget =CommonListView_OptionsList->GetEntryWidgetFromItem<UWidget_ListEntry_Base>(InHoveredItem);

	checkf(HoveredEntryWidget, TEXT("Invalid Widget for HoveredItem"));

	HoveredEntryWidget->NativeOnListEntryWidgetHovered(bWasHovered);

	if (bWasHovered)
	{
		DetailsView_EntryInfo->UpdateDetailsViewInfo(
				CastChecked<UListDataObject_Base>(InHoveredItem),
				TryGetEntryWidgetClassName(InHoveredItem)
			);
	}
	else
	{
		if (UListDataObject_Base* SelectedItem = CommonListView_OptionsList->GetSelectedItem<UListDataObject_Base>())
		{
			DetailsView_EntryInfo->UpdateDetailsViewInfo(
				SelectedItem,
				TryGetEntryWidgetClassName(SelectedItem)
				);
		}
	}
}

void UWidget_OptionsScreen::OnListViewItemSelected(UObject* InSelectedItem)
{
	if (!InSelectedItem) return;

	DetailsView_EntryInfo->UpdateDetailsViewInfo(
			CastChecked<UListDataObject_Base>(InSelectedItem),
			TryGetEntryWidgetClassName(InSelectedItem)
		);
}

FString UWidget_OptionsScreen::TryGetEntryWidgetClassName(UObject* InOwningListItem) const
{
	if (UUserWidget* FoundWidget = CommonListView_OptionsList->GetEntryWidgetFromItem(InOwningListItem))
	{
		return FoundWidget->GetClass()->GetName();
	}
	return TEXT("Entry widget not valid");
}

void UWidget_OptionsScreen::OnListViewListDataModified(UListDataObject_Base* ModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	if (!ModifiedData || bIsResettingData) return;

	if (ModifiedData->CanResetBackToDefaultValue())
	{
		ResettableDataArray.AddUnique(ModifiedData);

		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
	else
	{
		if (ResettableDataArray.Contains(ModifiedData))
		{
			ResettableDataArray.Remove(ModifiedData);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
}

void UWidget_OptionsScreen::OnOptionsTabSelected(FName TabId)
{
	DetailsView_EntryInfo->ClearDetailsViewInfo();
	
	TArray<UListDataObject_Base*> FoundListSourceItems = GetOrCreateDataRegistry()->GetListSourceItemsBySelectedTabID(TabId);

	CommonListView_OptionsList->SetListItems<UListDataObject_Base*>(FoundListSourceItems);
	CommonListView_OptionsList->RequestRefresh();

	if (CommonListView_OptionsList->GetNumItems() != 0)
	{
		CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
	}

	ResettableDataArray.Empty();
	for (UListDataObject_Base* FoundItem : FoundListSourceItems)
	{
		if (!FoundItem) continue;

		if (!FoundItem->OnListDataModified.IsBound())
		{
			FoundItem->OnListDataModified.AddUObject(this, &ThisClass::OnListViewListDataModified);
		}

		if (FoundItem->CanResetBackToDefaultValue())
		{
			ResettableDataArray.AddUnique(FoundItem);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
	else
	{
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
}

