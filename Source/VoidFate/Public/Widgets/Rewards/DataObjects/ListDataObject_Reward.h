// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "ListDataObject_Reward.generated.h"

class UPrimaryDataAsset_Reward;
/**
 * 專門用來將肉鴿獎勵 Data Asset 包裝成 ListView 認識的 Data Object
 */
UCLASS()
class VOIDFATE_API UListDataObject_Reward : public UListDataObject_Base
{
	GENERATED_BODY()

public:
	// 傳入 Data Asset 進行初始化
	void InitRewardData(UPrimaryDataAsset_Reward* InRewardAsset);

	// 預留 Getter，萬一 UI 或是其他邏輯需要用到最原始的 Data Asset
	UPrimaryDataAsset_Reward* GetRewardAsset() const { return RewardAsset; }

private:
	// 暫存原始的 Data Asset
	UPROPERTY(Transient)
	UPrimaryDataAsset_Reward* RewardAsset;
};
