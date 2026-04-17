// Paradise NiseMono All Rights Reserved


#include "Widgets/Options/Widget_KeyRemapScreen.h"

#include "CommonInputSubsystem.h"
#include "CommonRichTextBlock.h"
#include "ICommonInputModule.h"
#include "VFDebugHelper.h"
#include "CommonUITypes.h"
#include "Framework/Application/IInputProcessor.h"
#include "VFFunctionLibrary.h"

class FKeyRemapScreenInputPreprocessor : public IInputProcessor
{
public:
	FKeyRemapScreenInputPreprocessor(ECommonInputType InInputTypeToListenTo, ULocalPlayer* InOwningLocalPlayer)
		: CachedInputTypeToListenTo(InInputTypeToListenTo), CachedWeakOwningLocalPlayer(InOwningLocalPlayer)
	{
		
	}

	DECLARE_DELEGATE_OneParam(FOnInputPreprocessorKeyPressedDelegate, const FKey& /*PressedKey*/);
	FOnInputPreprocessorKeyPressedDelegate OnInputPreprocessorKeyPressed;

	DECLARE_DELEGATE_OneParam(FOnInputPreprocessorKeySelectCanceledDelegate, const FString& /*CanceledReason*/);
	FOnInputPreprocessorKeySelectCanceledDelegate OnInputPreprocessorKeySelectCanceled;
protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{
		
	}
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		ProcessPressedKey(InKeyEvent.GetKey());

		return true;
	}
	virtual bool HandleMouseButtonDownEvent( FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		ProcessPressedKey(MouseEvent.GetEffectingButton());

		return true;
	}
	void ProcessPressedKey(const FKey& InPressedKey)
	{
		if (InPressedKey == EKeys::Escape)
		{
			OnInputPreprocessorKeySelectCanceled.ExecuteIfBound(UVFFunctionLibrary::GetOptionsScreenDescriptionText("KeyRemapCancelKey").ToString());

			return;
		}

		UCommonInputSubsystem* CommonInputSubsystem =UCommonInputSubsystem::Get(CachedWeakOwningLocalPlayer.Get());
		
		check(CommonInputSubsystem);

		ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();

		switch (CachedInputTypeToListenTo)
		{
		case ECommonInputType::MouseAndKeyboard:
			if (InPressedKey.IsGamepadKey() || CurrentInputType == ECommonInputType::Gamepad)
			{
				OnInputPreprocessorKeySelectCanceled.ExecuteIfBound(UVFFunctionLibrary::GetOptionsScreenDescriptionText("DetectedGamepadWarnKey").ToString());

				return;
			}
				
			break;
		case ECommonInputType::Gamepad:

			if (CurrentInputType == ECommonInputType::Gamepad && InPressedKey == EKeys::LeftMouseButton)
			{
				FCommonInputActionDataBase* InputActionData = ICommonInputModule::GetSettings().GetDefaultClickAction().GetRow<FCommonInputActionDataBase>(TEXT(""));

				check(InputActionData);

				OnInputPreprocessorKeyPressed.ExecuteIfBound(InputActionData->GetDefaultGamepadInputTypeInfo().GetKey());

				return;
			}
			
			if (!InPressedKey.IsGamepadKey())
			{
				OnInputPreprocessorKeySelectCanceled.ExecuteIfBound(UVFFunctionLibrary::GetOptionsScreenDescriptionText("DetectedNonGamepadWarnKey").ToString());

				return;
			}
			break;
		default:
			break;
		}

		OnInputPreprocessorKeyPressed.ExecuteIfBound(InPressedKey);
	}

private:
	ECommonInputType CachedInputTypeToListenTo;
	TWeakObjectPtr<ULocalPlayer> CachedWeakOwningLocalPlayer;
};

void UWidget_KeyRemapScreen::SetDesiredInputTypeToFilter(ECommonInputType InDesiredInputType)
{
	CachedDesiredInputType = InDesiredInputType;
}

void UWidget_KeyRemapScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	CachedInputPreprocessor = MakeShared<FKeyRemapScreenInputPreprocessor>(CachedDesiredInputType, GetOwningLocalPlayer());
	CachedInputPreprocessor->OnInputPreprocessorKeyPressed.BindUObject(this, &ThisClass::OnValidKeyPressedDetected);
	CachedInputPreprocessor->OnInputPreprocessorKeySelectCanceled.BindUObject(this, &ThisClass::OnKeySelectedCanceled);

	FSlateApplication::Get().RegisterInputPreProcessor(CachedInputPreprocessor, -1);

	FText InputDeviceNameText;

	switch (CachedDesiredInputType)
	{
	case ECommonInputType::MouseAndKeyboard:
		InputDeviceNameText = UVFFunctionLibrary::GetOptionsScreenDescriptionText("MouseAndKeyboardKey");
		break;
	case ECommonInputType::Gamepad:
		InputDeviceNameText = UVFFunctionLibrary::GetOptionsScreenDescriptionText("GamepadKey");
		break;
	default:
		break;
	}

	FText RawText = UVFFunctionLibrary::GetOptionsScreenDescriptionText("PressAnyKey");

	FFormatNamedArguments Args;
	Args.Add(TEXT("DeviceName"), InputDeviceNameText);

	FText FormattedText = FText::Format(RawText, Args);

	CommonRichText_RemapMessage->SetText(FormattedText);
}

void UWidget_KeyRemapScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (CachedInputPreprocessor)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(CachedInputPreprocessor);

		CachedInputPreprocessor.Reset();
	}
}

void UWidget_KeyRemapScreen::OnValidKeyPressedDetected(const FKey& PressedKey)
{
	RequestDeactivateWidget(
		[this, PressedKey]()
		{
			OnKeyRemapScreenKeyPressed.ExecuteIfBound(PressedKey);
		}
	);
}

void UWidget_KeyRemapScreen::OnKeySelectedCanceled(const FString& CanceledReason)
{
	RequestDeactivateWidget(
		[this, CanceledReason]()
		{
			OnKeyRemapScreenKeySelectCanceled.ExecuteIfBound(CanceledReason);
		}
	);
}

void UWidget_KeyRemapScreen::RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback)
{
	FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateLambda(
			[PreDeactivateCallback, this](float DeltaTime)->bool
			{
				PreDeactivateCallback();
				
				DeactivateWidget();

				return false;
			}
		)
	);
}
