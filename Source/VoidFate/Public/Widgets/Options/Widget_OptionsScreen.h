// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "VFTypes/VFEnumTypes.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_OptionsScreen.generated.h"

class UListDataObject_Base;
class UWidget_OptionsDetailsView;
class UVFCommonListView;
class UVFTabListWidgetBase;
class UOptionsDataRegistry;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class VOIDFATE_API UWidget_OptionsScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

protected:
	// ~Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	// ~End UUserWidget Interface

	// ~Begin UCommonActivatableWidget Interface
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	// ~End UCommonActivatableWidget Interface
	
private:
	UOptionsDataRegistry* GetOrCreateDataRegistry();
	
	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

	void OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered);
	void OnListViewItemSelected(UObject* InSelectedItem);

	FString TryGetEntryWidgetClassName(UObject* InOwningListItem) const;

	void OnListViewListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason);

	UFUNCTION()
	void OnOptionsTabSelected(FName TabId);

	//***** Bound Widgets *****//
	UPROPERTY(meta=(BindWidget))
	UVFTabListWidgetBase* TabListWidget_OptionsTab;

	UPROPERTY(meta=(BindWidget))
	UVFCommonListView* CommonListView_OptionsList;

	UPROPERTY(meta=(BindWidget))
	UWidget_OptionsDetailsView* DetailsView_EntryInfo;
	//***** Bound Widgets *****//

	//Handle the creation of data in options screen. Direct access to this variable is forbidden.
	UPROPERTY(Transient)
	UOptionsDataRegistry* CreatedOwningDataRegistry;
	
	UPROPERTY(EditDefaultsOnly, Category = "Frontend Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;

	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> ResettableDataArray;

	bool bIsResettingData = false;
};
