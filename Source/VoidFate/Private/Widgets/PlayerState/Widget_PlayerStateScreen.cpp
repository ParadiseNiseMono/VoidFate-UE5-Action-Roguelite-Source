// Paradise NiseMono All Rights Reserved


#include "Widgets/PlayerState/Widget_PlayerStateScreen.h"
#include "CommonActivatableWidgetSwitcher.h"
#include "ICommonInputModule.h"
#include "Input/CommonUIInputTypes.h"
#include "Widgets/Components/VFTabListWidgetBase.h"
#include "Widgets/Options/Widget_OptionsDetailsView.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"
#include "Widgets/PlayerState/Widget_PlayerState_Rewards.h"
#include "Widgets/PlayerState/Widget_PlayerState_Stats.h"
#include "Widgets/PlayerState/Widget_PlayerStateDetailsView.h"
#include "VFFunctionLibrary.h"

void UWidget_PlayerStateScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TabListWidget_StateTab->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnStateTabSelected);

	RegisterUIActionBinding(
	FBindUIActionArgs(
	ICommonInputModule::GetSettings().GetDefaultBackAction(),
	true,
	FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
		)
	);

	if (Page_RewardsPanel)
	{
		Page_RewardsPanel->OnRewardItemHovered.AddUniqueDynamic(this, &ThisClass::OnListViewItemHovered);
	}

	ProtagonistOverviewData = NewObject<UListDataObject_StaticInfo>(this);
	
	ProtagonistOverviewData->SetDataDisplayName(UVFFunctionLibrary::GetPlayerStateScreenText("AttributeIntroKey"));
	
	FText OverviewDesc = UVFFunctionLibrary::GetPlayerStateScreenText("AttributeDescKey");
	ProtagonistOverviewData->SetDescriptionRichText(FText(OverviewDesc));
	
}

void UWidget_PlayerStateScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (TabListWidget_StateTab->GetTabButtonBaseByID(FName("Tab_Stats")) == nullptr)
	{
		TabListWidget_StateTab->RequestRegisterTab(FName("Tab_Stats"), UVFFunctionLibrary::GetPlayerStateScreenText("AttributesKey"));
		TabListWidget_StateTab->RequestRegisterTab(FName("Tab_Rewards"), UVFFunctionLibrary::GetPlayerStateScreenText("RewardsKey"));
	}
	
	TabListWidget_StateTab->SelectTabByID(FName("Tab_Stats"));
}

void UWidget_PlayerStateScreen::OnStateTabSelected(FName TabId)
{
	DetailsView_GlobalInfo->ClearDetailsViewInfo();
	if (TabId == FName("Tab_Stats"))
	{
		if (Switcher_StatePages && Page_StatsPanel)
		{
			Switcher_StatePages->SetActiveWidget(Page_StatsPanel);
			SetDesiredFocusWidget(Page_StatsPanel);
			
			if (DetailsView_GlobalInfo && ProtagonistOverviewData)
			{
				DetailsView_GlobalInfo->UpdateDetailsViewInfo(ProtagonistOverviewData);
			}
		}
	}
	else if (TabId == FName("Tab_Rewards"))
	{
		if (Switcher_StatePages && Page_RewardsPanel)
		{
			SetDesiredFocusWidget(Page_RewardsPanel);
			Switcher_StatePages->SetActiveWidget(Page_RewardsPanel);
		}
	}
}

void UWidget_PlayerStateScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}

void UWidget_PlayerStateScreen::OnListViewItemHovered(UListDataObject_Base* HoveredData)
{
	if (DetailsView_GlobalInfo)
	{
		if (HoveredData)
		{
			DetailsView_GlobalInfo->UpdateDetailsViewInfo(HoveredData);
		}
		else
		{
			DetailsView_GlobalInfo->ClearDetailsViewInfo();
		}
	}
	
}

