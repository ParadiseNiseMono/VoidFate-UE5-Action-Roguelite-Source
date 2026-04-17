// Paradise NiseMono All Rights Reserved


#include "Widgets/VFStatBarWidget.h"

#include "Components/ProgressBar.h"

UVFStatBarWidget::UVFStatBarWidget()
{
	bSupportsActivationFocus = false;
	bAutoRestoreFocus = false;
}

void UVFStatBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Bar_Ghost)
	{
		if (!FMath::IsNearlyEqual(GhostPercent, TargetPercent, 0.00001f))
		{
			GhostPercent = FMath::FInterpTo(GhostPercent, TargetPercent, InDeltaTime, GhostInterpSpeed);
			Bar_Ghost->SetPercent(GhostPercent);
		}
	}
}

void UVFStatBarWidget::UpdateValues(float CurrentValue, float MaxValue)
{
	if (MaxValue <= 0.f) return;
	
	float NewPercent = FMath::Clamp(CurrentValue / MaxValue, 0.f, 1.f);
	
	if (Bar_Current)
	{
		Bar_Current->SetPercent(NewPercent);
		BP_OnStatBarPercentChanged(NewPercent);
	}

	if (NewPercent < TargetPercent)
	{
		BP_OnStateBarDecreased();
	}

	if (NewPercent > TargetPercent)
	{
		BP_OnStateBarIncreased();
	}
	
	TargetPercent = NewPercent;
}

