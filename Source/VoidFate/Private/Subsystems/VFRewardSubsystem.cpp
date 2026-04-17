// Paradise NiseMono All Rights Reserved


#include "Subsystems/VFRewardSubsystem.h"

#include "VFDebugHelper.h"
#include "DataAssets/Reward/PrimaryDataAsset_Reward.h"
#include "Engine/AssetManager.h"

UVFRewardSubsystem* UVFRewardSubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		return World->GetSubsystem<UVFRewardSubsystem>();
	}

	return nullptr;
}

void UVFRewardSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	AcquiredRewardIDs.Empty();
	SynergyTagCounts.Empty();
	ExhaustedRewardIDs.Empty();
}

void UVFRewardSubsystem::Deinitialize()
{
	AcquiredRewardIDs.Empty();
	SynergyTagCounts.Empty();
	ExhaustedRewardIDs.Empty();
}

TArray<FPrimaryAssetId> UVFRewardSubsystem::GetRandomRewardIDs(int32 Count)
{
	TArray<FPrimaryAssetId> SelectedIDs;

	UAssetManager& AssetManager = UAssetManager::Get();
	if (!AssetManager.IsInitialized()) return SelectedIDs;

	TArray<FPrimaryAssetId> AllRewardIDs;
	AssetManager.GetPrimaryAssetIdList(FName("VFReward"), AllRewardIDs);

	// 定義一個內部結構體，把候選的 ID、Asset 和算出來的最終權重綁在一起
	struct FRewardCandidate
	{
		FPrimaryAssetId ID;
		float FinalWeight;
	};

	TArray<FRewardCandidate> Candidates;

	for (const FPrimaryAssetId& ID : AllRewardIDs)
	{
		if (ExhaustedRewardIDs.Contains(ID)) continue;

		// 確保物件即使沒在記憶體裡，也能被正確讀取
		UObject* LoadedObject = AssetManager.GetPrimaryAssetObject(ID);
		if (!LoadedObject)
		{
			// 如果沒載入，透過路徑同步強制載入 (PDA 容量極小，同步載入不會卡頓)
			FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ID);
			if (AssetPath.IsValid())
			{
				LoadedObject = AssetPath.TryLoad();
			}
		}

		UPrimaryDataAsset_Reward* RewardAsset = Cast<UPrimaryDataAsset_Reward>(LoadedObject);
		if (!RewardAsset) continue;

		float Weight = RewardAsset->BaseDropWeight;

		if (RewardAsset->SynergyTag.IsValid())
		{
			if (const int32* StackCount = SynergyTagCounts.Find(RewardAsset->SynergyTag))
			{
				Weight *= (1.0f + (*StackCount * RewardAsset->SynergyWeightBonusPerStack));
			}
		}

		Candidates.Add({ ID, Weight });
	}

	int32 ActualCount = FMath::Min(Count, Candidates.Num());
	if (ActualCount <= 0) return SelectedIDs;

	for (int32 i = 0; i < ActualCount; ++i)
	{
		// 算出當前池子裡所有權重的總和
		float TotalWeight = 0.0f;
		for (const FRewardCandidate& Candidate : Candidates)
		{
			TotalWeight += Candidate.FinalWeight;
		}

		float RandomPoint = FMath::FRandRange(0.0f, TotalWeight);
		float AccumulatedWeight = 0.0f;
		int32 SelectedIndex = -1;

		for (int32 j = 0; j < Candidates.Num(); ++j)
		{
			AccumulatedWeight += Candidates[j].FinalWeight;
			if (AccumulatedWeight >= RandomPoint)
			{
				SelectedIndex = j;
				//Debug::Print(FString::Printf(TEXT("抽獎第 %d/%d: 總權重=%.2f, 隨機點=%.2f, 選中 %s (權重=%.2f)"), i + 1, ActualCount, TotalWeight, RandomPoint, *Candidates[j].ID.ToString(), Candidates[j].FinalWeight));
				break;
			}
		}

		if (SelectedIndex == -1) SelectedIndex = Candidates.Num() - 1;

		SelectedIDs.Add(Candidates[SelectedIndex].ID);

		// 把抽中的獎勵移出池子，避免這次的三選一出現兩個一模一樣的選項
		Candidates.RemoveAt(SelectedIndex);
	}

	return SelectedIDs;
}

void UVFRewardSubsystem::RecordAcquiredReward(UPrimaryDataAsset_Reward* AcquiredReward)
{
	if (!AcquiredReward) return;

	FPrimaryAssetId RewardID = AcquiredReward->GetPrimaryAssetId();
    
	AcquiredRewardIDs.Add(RewardID);

	if (AcquiredReward->SynergyTag.IsValid())
	{
		int32& Count = SynergyTagCounts.FindOrAdd(AcquiredReward->SynergyTag);

		Count++;
	}
	
	if (!AcquiredReward->bIsRepeatable)
	{
		ExhaustedRewardIDs.AddUnique(RewardID);
	}
}
