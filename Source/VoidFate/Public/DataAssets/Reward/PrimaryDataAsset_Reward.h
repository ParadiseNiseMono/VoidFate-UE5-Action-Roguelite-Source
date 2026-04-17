// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "PrimaryDataAsset_Reward.generated.h"

class UGameplayEffect;
class UGameplayAbility;

USTRUCT(BlueprintType)
struct FVFRewardUIData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FText RewardName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (MultiLine = "true"))
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> RewardIcon;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class VOIDFATE_API UPrimaryDataAsset_Reward : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	// 這個獎勵的專屬 ID 標籤 (例如 Reward.Generic.Common.PowerBoost)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Identification", meta = (Categories = "Reward"))
	FGameplayTag RewardTag;

	// 這個獎勵所屬的「大流派」標籤 (例如 Reward.Combat)【Generic類別禁止加入，請直接空著】
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Identification", meta = (Categories = "Reward"))
	FGameplayTag SynergyTag;

	//將這個獎勵的資訊顯示到UI上的資料
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Presentation")
	FVFRewardUIData UIData;

	//這個獎勵可以賦予玩家的特殊效果
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Logic")
	TArray<TSoftClassPtr<UGameplayEffect>> EffectsToGrant;

	//這個獎勵可以賦予玩家的能力
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Logic")
	TArray<TSoftClassPtr<UGameplayAbility>> AbilitiesToGrant;

	//獲取這個獎勵之前所需要的前置條件
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Prerequisites")
	FGameplayTagContainer RequiredTags;

	//是否可以重複獲取該能力
	UPROPERTY(EditDefaultsOnly, Category = "Logic")
	bool bIsRepeatable = false;

	// 這個獎勵的基礎抽中權重 (數值越高越容易抽中)
	UPROPERTY(EditDefaultsOnly, Category = "Logic")
	float BaseDropWeight = 100.0f;

	// 這個獎勵專屬的流派共鳴倍率加成
	// (例如：普通卡設為 0.1，超稀有卡設為 2.0)
	UPROPERTY(EditDefaultsOnly, Category = "Logic")
	float SynergyWeightBonusPerStack = 0.2f;
};
