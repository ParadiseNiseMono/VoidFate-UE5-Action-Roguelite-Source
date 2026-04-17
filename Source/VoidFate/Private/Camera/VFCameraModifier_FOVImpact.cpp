// Paradise NiseMono All Rights Reserved


#include "Camera/VFCameraModifier_FOVImpact.h"

UVFCameraModifier_FOVImpact::UVFCameraModifier_FOVImpact()
{
	Priority = 127;
	TargetFOVOffset = 0.0f;
	ImpactDuration = 0.2f;
	ElapsedTime = 0.0f;
	ImpactCurve = nullptr;
}

void UVFCameraModifier_FOVImpact::InitFOVImpact(float InFOVOffset, float InDuration, UCurveFloat* InImpactCurve)
{
	TargetFOVOffset = InFOVOffset;
	ImpactDuration = InDuration;
	ImpactCurve = InImpactCurve;
	ElapsedTime = 0.0f;
}

bool UVFCameraModifier_FOVImpact::ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV)
{
	Super::ModifyCamera(DeltaTime, InOutPOV);

	if (ElapsedTime >= ImpactDuration || !ImpactCurve)
	{
		// 衝擊結束，讓這個 Modifier 自動把自己銷毀關閉
		DisableModifier(true);
		return false;
	}

	float CurrentAlpha = ElapsedTime / ImpactDuration;

	float CurveValue = ImpactCurve->GetFloatValue(CurrentAlpha);

	InOutPOV.FOV += (TargetFOVOffset * CurveValue);

	ElapsedTime += DeltaTime;

	return false;
}
