// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonButton_RewardCard.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRewardCardClickedDelegate, UPrimaryDataAsset_Reward*, SelectedReward);

class UPrimaryDataAsset_Reward;
class UCommonLazyImage;
class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class VOIDFATE_API UCommonButton_RewardCard : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Void|UI")
	void InitCard(UPrimaryDataAsset_Reward* InRewardAsset);
	
	UPROPERTY(BlueprintAssignable, Category = "Void|Events")
	FOnRewardCardClickedDelegate OnRewardCardClicked;

protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void HandleRewardButtonClicked(UCommonButtonBase* Button);
	
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Title;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Description;

	UPROPERTY(meta = (BindWidget))
	UCommonLazyImage* CommonLazyImage_Icon;

private:
	UPROPERTY(Transient)
	UPrimaryDataAsset_Reward* CachedRewardAsset;
};
