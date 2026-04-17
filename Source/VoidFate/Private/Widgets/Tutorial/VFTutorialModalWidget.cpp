// Paradise NiseMono All Rights Reserved


#include "Widgets/Tutorial/VFTutorialModalWidget.h"
#include "Kismet/GameplayStatics.h"
#include "CommonInputSubsystem.h"

void UVFTutorialModalWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (AVFTutorialGameMode* TutorialGM = Cast<AVFTutorialGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		TutorialGM->OnTutorialPhaseChanged.AddUniqueDynamic(this, &ThisClass::HandlePhaseChanged);

		OnUpdateTutorialUI(TutorialGM->GetCurrentTutorialPhase());
	}
	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		if (UCommonInputSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UCommonInputSubsystem>())
		{
			InputSubsystem->OnInputMethodChangedNative.AddUObject(this, &ThisClass::HandleInputMethodChanged);

			OnInputDeviceUpdated(InputSubsystem->GetCurrentInputType());
		}
	}
}

void UVFTutorialModalWidget::HandlePhaseChanged(EVFTutorialPhase OldPhase, EVFTutorialPhase NewPhase)
{
	OnUpdateTutorialUI(NewPhase);
}

void UVFTutorialModalWidget::HandleInputMethodChanged(ECommonInputType CurrentInputType)
{
	OnInputDeviceUpdated(CurrentInputType);
}
