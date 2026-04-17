// Paradise NiseMono All Rights Reserved


#include "Widgets/Widget_ConfirmScreen.h"

#include "CommonTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "Widgets/Components/VFCommonButtonBase.h"
#include "VFFunctionLibrary.h"

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKScreen(const FText& InScreenTitle,
                                                                   const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;

	FConfirmScreenButtonInfo OKScreenButtonInfo;
	OKScreenButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Closed;
	OKScreenButtonInfo.ButtonTextToDisplay = UVFFunctionLibrary::GetModalScreenTitleText("OKKey");

	InfoObject->AvailableScreenButtons.Add(OKScreenButtonInfo);

	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& InScreenTitle,
	const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;

	FConfirmScreenButtonInfo YesScreenButtonInfo;
	YesScreenButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	YesScreenButtonInfo.ButtonTextToDisplay = UVFFunctionLibrary::GetModalScreenTitleText("YesKey");

	FConfirmScreenButtonInfo NoScreenButtonInfo;
	NoScreenButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Canceled;
	NoScreenButtonInfo.ButtonTextToDisplay = UVFFunctionLibrary::GetModalScreenTitleText("NoKey");

	InfoObject->AvailableScreenButtons.Add(YesScreenButtonInfo);
	InfoObject->AvailableScreenButtons.Add(NoScreenButtonInfo);

	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOkCancelScreen(const FText& InScreenTitle,
	const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;

	FConfirmScreenButtonInfo OKScreenButtonInfo;
	OKScreenButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	OKScreenButtonInfo.ButtonTextToDisplay = UVFFunctionLibrary::GetModalScreenTitleText("OKKey");

	FConfirmScreenButtonInfo CancelScreenButtonInfo;
	CancelScreenButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Canceled;
	CancelScreenButtonInfo.ButtonTextToDisplay = UVFFunctionLibrary::GetModalScreenTitleText("CancelKey");

	InfoObject->AvailableScreenButtons.Add(OKScreenButtonInfo);
	InfoObject->AvailableScreenButtons.Add(CancelScreenButtonInfo);

	return InfoObject;
}

void UWidget_ConfirmScreen::InitConfirmScreen(UConfirmScreenInfoObject* InScreenInfoObject,
	TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback)
{
	check(InScreenInfoObject && CommonTextBlock_Message && CommonTextBlock_Title && DynamicEntryBox_Buttons);

	CommonTextBlock_Title->SetText(InScreenInfoObject->ScreenTitle);
	CommonTextBlock_Message->SetText(InScreenInfoObject->ScreenMessage);

	//Checking if the entry box has old button created previously
	if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		/*
		 * Clearing the old buttons the entry box has the widget type for the entry box
		 * is specified in the child widget blueprint.
		 */
		DynamicEntryBox_Buttons->Reset<UVFCommonButtonBase>(
			[](UVFCommonButtonBase& ExistingButton)
		{
			ExistingButton.OnClicked().Clear();
		}
		);
	}

	check(!InScreenInfoObject->AvailableScreenButtons.IsEmpty());

	for (const FConfirmScreenButtonInfo& ButtonInfo : InScreenInfoObject->AvailableScreenButtons)
	{
		UVFCommonButtonBase* AddedButton = DynamicEntryBox_Buttons->CreateEntry<UVFCommonButtonBase>();

		AddedButton->SetButtonText(ButtonInfo.ButtonTextToDisplay);
		AddedButton->OnClicked().AddLambda(
			[ClickedButtonCallback, ButtonInfo, this]()
			{
				ClickedButtonCallback(ButtonInfo.ConfirmScreenButtonType);

				DeactivateWidget();
			}
			);
	}
}

UWidget* UWidget_ConfirmScreen::NativeGetDesiredFocusTarget() const
{
	if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		/*
		 * Set Focus on the last button.
		 */
		DynamicEntryBox_Buttons->GetAllEntries().Last()->SetFocus();
	}
	
	return Super::NativeGetDesiredFocusTarget();
}
