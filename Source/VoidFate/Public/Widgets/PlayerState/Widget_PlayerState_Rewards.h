// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_PlayerState_Rewards.generated.h"

class UListDataObject_Base;
class UVFCommonListView;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRewardItemHoveredDelegate, UListDataObject_Base*, HoveredData);

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class VOIDFATE_API UWidget_PlayerState_Rewards : public UWidget_ActivatableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Void|Events")
	FOnRewardItemHoveredDelegate OnRewardItemHovered;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

private:
	void OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered);
	void OnListViewItemSelected(UObject* InSelectedItem);
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UVFCommonListView* CommonListView_RewardList;

	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> CachedRewardDataObjects;
};
