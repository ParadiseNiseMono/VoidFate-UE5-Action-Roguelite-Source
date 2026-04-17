// Paradise NiseMono All Rights Reserved


#include "Widgets/PlayerState/Widget_PlayerState_Rewards.h"

#include "DataAssets/Reward/PrimaryDataAsset_Reward.h"
#include "Engine/AssetManager.h"
#include "Subsystems/VFRewardSubsystem.h"
#include "Widgets/Components/VFCommonListView.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"
#include "Widgets/Rewards/DataObjects/ListDataObject_Reward.h"

void UWidget_PlayerState_Rewards::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonListView_RewardList->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::OnListViewItemHovered);
	CommonListView_RewardList->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnListViewItemSelected);
}

void UWidget_PlayerState_Rewards::NativeOnActivated()
{
	Super::NativeOnActivated();

	CachedRewardDataObjects.Empty();

	UVFRewardSubsystem* RewardSubsystem = GetWorld()->GetSubsystem<UVFRewardSubsystem>();
	if (!RewardSubsystem) return;

	TArray<FPrimaryAssetId> AcquiredIDs = RewardSubsystem->GetAllAcquiredRewards();
	UAssetManager& AssetManager = UAssetManager::Get();

	for (const FPrimaryAssetId& ID : AcquiredIDs)
	{
		if (UPrimaryDataAsset_Reward* RewardAsset = Cast<UPrimaryDataAsset_Reward>(AssetManager.GetPrimaryAssetPath(ID).TryLoad()))
		{
			UListDataObject_Reward* NewRewardData = NewObject<UListDataObject_Reward>(this);
			NewRewardData->InitRewardData(RewardAsset);
			
			CachedRewardDataObjects.Add(NewRewardData);
		}
	}

	CommonListView_RewardList->SetListItems<UListDataObject_Base*>(CachedRewardDataObjects);
	CommonListView_RewardList->RequestRefresh();

	if (CommonListView_RewardList->GetNumItems() > 0)
	{
		CommonListView_RewardList->NavigateToIndex(0);
		CommonListView_RewardList->SetSelectedIndex(0);
	}
}

UWidget* UWidget_PlayerState_Rewards::NativeGetDesiredFocusTarget() const
{
	if (UObject* SelectedObject = CommonListView_RewardList->GetSelectedItem())
	{
		if (UUserWidget* SelectedWidget = CommonListView_RewardList->GetEntryWidgetFromItem(SelectedObject))
		{
			return SelectedWidget;
		}
	}
	return Super::NativeGetDesiredFocusTarget();
}

void UWidget_PlayerState_Rewards::OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered)
{
	if (!InHoveredItem) return;
	
	UWidget_ListEntry_Base* HoveredEntryWidget = CommonListView_RewardList->GetEntryWidgetFromItem<UWidget_ListEntry_Base>(InHoveredItem);
	if (HoveredEntryWidget)
	{
		HoveredEntryWidget->NativeOnListEntryWidgetHovered(bWasHovered);
	}

	if (bWasHovered)
	{
		OnRewardItemHovered.Broadcast(CastChecked<UListDataObject_Base>(InHoveredItem));
	}
	else
	{
		if (UListDataObject_Base* SelectedItem = CommonListView_RewardList->GetSelectedItem<UListDataObject_Base>())
		{
			OnRewardItemHovered.Broadcast(SelectedItem);
		}
	}
}

void UWidget_PlayerState_Rewards::OnListViewItemSelected(UObject* InSelectedItem)
{
	if (!InSelectedItem) return;
	OnRewardItemHovered.Broadcast(CastChecked<UListDataObject_Base>(InSelectedItem));
}
