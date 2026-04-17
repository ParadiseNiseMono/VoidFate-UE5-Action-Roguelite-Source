// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_Reward.generated.h"

class UVFCommonButtonBase;
class UListDataObject_Reward;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class VOIDFATE_API UWidget_ListEntry_Reward : public UWidget_ListEntry_Base
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	
	// ~Begin UWidget_ListEntry_Base Interface
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;
	// ~End UWidget_ListEntry_Base Interface

private:
	UFUNCTION()
	void OnBackgroundButtonClicked();

	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess = "true"))
	UVFCommonButtonBase* CommonButton_Background;
	//***** Bound Widgets *****//
	
	// 暫存專屬的 Reward Data Object，方便未來擴充使用
	UPROPERTY(Transient)
	UListDataObject_Reward* CachedOwningRewardDataObject;
};
