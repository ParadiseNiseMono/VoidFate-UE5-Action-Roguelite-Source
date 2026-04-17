// Paradise NiseMono All Rights Reserved


#include "Widgets/Options/ListEntries/Widget_ListEntry_KeyRemap.h"

#include "VFDebugHelper.h"
#include "VFFunctionLibrary.h"
#include "VFGameplayTags.h"
#include "Subsystems/VFUISubsystem.h"
#include "Widgets/Components/VFCommonButtonBase.h"
#include "Widgets/Options/Widget_KeyRemapScreen.h"
#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"

void UWidget_ListEntry_KeyRemap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_RemapKey->OnClicked().AddUObject(this, &ThisClass::OnRemapKeyButtonClicked);
	CommonButton_ResetKeyBinding->OnClicked().AddUObject(this, &ThisClass::OnResetKeyBindingButtonClicked);
}

void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningKeyRemapDataObject = CastChecked<UListDataObject_KeyRemap>(InOwningListDataObject);

	CommonButton_RemapKey->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
}

void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CommonButton_RemapKey->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
		UVFUISubsystem::Get(this)->OnKeybindingChanged.Broadcast();
	}
}

void UWidget_ListEntry_KeyRemap::OnRemapKeyButtonClicked()
{
	UVFUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		VFGameplayTags::Frontend_WidgetStack_Modal,
		UVFFunctionLibrary::GetFrontendSoftWidgetClassByTag(VFGameplayTags::Frontend_Widget_KeyRemapScreen),
		[this](EAsyncPushWidgetState PushState, UWidget_ActivatableBase* PushedWidget)
		{
			if (PushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UWidget_KeyRemapScreen* CreatedKeyRemapScreen = CastChecked<UWidget_KeyRemapScreen>(PushedWidget);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeyPressed.BindUObject(this, &ThisClass::OnKeyToRemapPressed);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeySelectCanceled.BindUObject(this, &ThisClass::OnKeyRemapCanceled);

				if (CachedOwningKeyRemapDataObject)
				{
					CreatedKeyRemapScreen->SetDesiredInputTypeToFilter(CachedOwningKeyRemapDataObject->GetDesiredInputKeyType());
				}
			}
		}
	);
}

void UWidget_ListEntry_KeyRemap::OnResetKeyBindingButtonClicked()
{
	SelectThisEntryWidget();

	if (!CachedOwningKeyRemapDataObject) return;
	
	//Check if the current key is already the default key. Display an OK Screen that says this is already the default value.
	if (!CachedOwningKeyRemapDataObject->CanResetBackToDefaultValue())
	{
		UVFUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
			EConfirmScreenType::Ok,
			FText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("ResetKeyMappingKey")),
			FText::FromString(UVFFunctionLibrary::GetOptionsScreenDescriptionText("TheKeyBindingForKey").ToString() + CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + UVFFunctionLibrary::GetOptionsScreenDescriptionText("IsAlreadySetToDefaultKey").ToString()),
			[](EConfirmScreenButtonType ClickedButton){}
		);

		return;
	}
	//Reset the key binding back to default
	UVFUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::OkCancel,
		FText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("ResetKeyMappingKey")),
		FText::FromString(UVFFunctionLibrary::GetOptionsScreenDescriptionText("AreYouSureResetKeyBindingKey").ToString() + CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + TEXT("?")),
		[this](EConfirmScreenButtonType ClickedButton)
		{
			if (ClickedButton == EConfirmScreenButtonType::Confirmed)
			{
				CachedOwningKeyRemapDataObject->TryResetBackToDefaultValue();
				UVFUISubsystem::Get(this)->OnKeybindingChanged.Broadcast();
			}
		}
	);
}

void UWidget_ListEntry_KeyRemap::OnKeyToRemapPressed(const FKey& PressedKey)
{
	SelectThisEntryWidget();
	
	if (CachedOwningKeyRemapDataObject)
	{
		CachedOwningKeyRemapDataObject->BindNewInputKey(PressedKey);
	}
}

void UWidget_ListEntry_KeyRemap::OnKeyRemapCanceled(const FString& CanceledReason)
{
	UVFUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::Ok,
		FText::FromString(TEXT("")),
		FText::FromString(CanceledReason),
		[](EConfirmScreenButtonType ConfirmButtonType){}
	);
}
