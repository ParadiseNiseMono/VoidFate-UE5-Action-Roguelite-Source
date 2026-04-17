// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_RewardScreen.generated.h"

class UPrimaryDataAsset_Reward;
class UCommonButton_RewardCard;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class VOIDFATE_API UWidget_RewardScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void BindAndAddRewardCard(UCommonButton_RewardCard* CardToAdd);
	
	UFUNCTION()
	void HandleRewardSelected(UPrimaryDataAsset_Reward* SelectedReward);
	
	TArray<UCommonButton_RewardCard*> RewardCards;

public:
	UFUNCTION(BlueprintPure)
	TArray<UCommonButton_RewardCard*> GetRewardCards();
};
