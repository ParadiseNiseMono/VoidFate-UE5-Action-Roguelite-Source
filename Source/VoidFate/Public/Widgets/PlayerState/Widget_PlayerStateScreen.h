// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_PlayerStateScreen.generated.h"

class UWidget_PlayerStateDetailsView;
class UWidget_PlayerState_Rewards;
class UListDataObject_Base;
class UVFCommonListView;
class UWidget_OptionsDetailsView;
class UCommonActivatableWidgetSwitcher;
class UVFTabListWidgetBase;
class UWidget_PlayerState_Stats;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class VOIDFATE_API UWidget_PlayerStateScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

protected:
	// ~Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	// ~End UUserWidget Interface

	// ~Begin UCommonActivatableWidget Interface
	virtual void NativeOnActivated() override;
	// ~End UCommonActivatableWidget Interface

private:
	UFUNCTION()
	void OnStateTabSelected(FName TabId);

	UFUNCTION()
	void OnBackBoundActionTriggered();

	UFUNCTION()
	void OnListViewItemHovered(UListDataObject_Base* HoveredData);

	//***** Bound Widgets *****//
	UPROPERTY(meta=(BindWidget))
	UVFTabListWidgetBase* TabListWidget_StateTab;

	UPROPERTY(meta=(BindWidget))
	UCommonActivatableWidgetSwitcher* Switcher_StatePages; 

	UPROPERTY(meta=(BindWidget))
	UWidget_PlayerState_Stats* Page_StatsPanel;

	UPROPERTY(meta=(BindWidget))
	UWidget_PlayerState_Rewards* Page_RewardsPanel;

	UPROPERTY(meta=(BindWidget))
	UWidget_PlayerStateDetailsView* DetailsView_GlobalInfo;
	//***** Bound Widgets *****//

	UPROPERTY(Transient)
	UListDataObject_Base* ProtagonistOverviewData;
};
