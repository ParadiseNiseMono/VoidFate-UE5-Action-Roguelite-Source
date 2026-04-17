// Paradise NiseMono All Rights Reserved


#include "Widgets/Rewards/CommonButton_RewardCard.h"

#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "DataAssets/Reward/PrimaryDataAsset_Reward.h"
#include "Subsystems/VFRewardSubsystem.h"

void UCommonButton_RewardCard::InitCard(UPrimaryDataAsset_Reward* InRewardAsset)
{
	if (!InRewardAsset) return;

	CachedRewardAsset = InRewardAsset;
	
	CommonTextBlock_Title->SetText(InRewardAsset->UIData.RewardName);
	CommonTextBlock_Description->SetText(InRewardAsset->UIData.Description);
	CommonLazyImage_Icon->SetBrushFromLazyTexture(InRewardAsset->UIData.RewardIcon);
}

void UCommonButton_RewardCard::NativeConstruct()
{
	Super::NativeConstruct();

	OnButtonBaseClicked.AddUniqueDynamic(this, &ThisClass::HandleRewardButtonClicked);
}

void UCommonButton_RewardCard::HandleRewardButtonClicked(UCommonButtonBase* Button)
{
	if (CachedRewardAsset)
	{
		OnRewardCardClicked.Broadcast(CachedRewardAsset);
	}
}

