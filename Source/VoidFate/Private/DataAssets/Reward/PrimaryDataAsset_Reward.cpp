// Paradise NiseMono All Rights Reserved


#include "DataAssets/Reward/PrimaryDataAsset_Reward.h"

FPrimaryAssetId UPrimaryDataAsset_Reward::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(FName("VFReward"), GetFName());
}
