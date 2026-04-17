// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "VFRewardSubsystem.generated.h"

class UPrimaryDataAsset_Reward;
/**
 * 
 */
UCLASS()
class VOIDFATE_API UVFRewardSubsystem : public	UWorldSubsystem
{
	GENERATED_BODY()

public:
	static UVFRewardSubsystem* Get(const UObject* WorldContextObject);

	// USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	/**
	 * 從 Asset Manager 的池子裡，隨機抽出指定數量的獎勵 ID
	 * @param Count 要抽出的數量 (例如 3)
	 * @return 抽中的 PrimaryAssetId 陣列
	 */
	UFUNCTION(BlueprintCallable, Category = "Void|Roguelite")
	TArray<FPrimaryAssetId> GetRandomRewardIDs(int32 Count);
	
	UFUNCTION(BlueprintCallable, Category = "Void|Roguelite")
	void RecordAcquiredReward(UPrimaryDataAsset_Reward* AcquiredReward);
	
	UFUNCTION(BlueprintPure, Category = "Void|Roguelite")
	TArray<FPrimaryAssetId> GetAllAcquiredRewards() const { return AcquiredRewardIDs; }

protected:
	// 記錄所有玩家獲得的獎勵 (供 UI 查看)
	UPROPERTY()
	TArray<FPrimaryAssetId> AcquiredRewardIDs;

	// 記錄「不可重複且已經獲得」的獎勵 (供抽卡過濾)
	UPROPERTY()
	TArray<FPrimaryAssetId> ExhaustedRewardIDs;

	// 記錄玩家目前已經擁有的各流派「具體數量」
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Void|Roguelite")
	TMap<FGameplayTag, int32> SynergyTagCounts;
};
