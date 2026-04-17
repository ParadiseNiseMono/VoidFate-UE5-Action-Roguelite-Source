// Paradise NiseMono All Rights Reserved


#include "Widgets/Rewards/DataObjects/ListDataObject_Reward.h"

#include "DataAssets/Reward/PrimaryDataAsset_Reward.h"

void UListDataObject_Reward::InitRewardData(UPrimaryDataAsset_Reward* InRewardAsset)
{
	if (!InRewardAsset) return;

	RewardAsset = InRewardAsset;
	
	SetDataID(InRewardAsset->GetPrimaryAssetId().PrimaryAssetName);
	SetDataDisplayName(InRewardAsset->UIData.RewardName);
	SetDescriptionRichText(InRewardAsset->UIData.Description);
	SetSoftDescriptionImage(InRewardAsset->UIData.RewardIcon);

	// 這只是顯示用的獎勵，不是遊戲設定，所以不會觸發 ApplySettings
	SetShouldApplySettingsImmediately(false);
}
