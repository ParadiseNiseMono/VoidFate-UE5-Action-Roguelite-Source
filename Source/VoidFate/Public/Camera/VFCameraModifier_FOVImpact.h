// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "VFCameraModifier_FOVImpact.generated.h"

/**
 * 專門負責在戰鬥中製造 FOV (視野) 縮放衝擊感的 Camera Modifier
 */
UCLASS()
class VOIDFATE_API UVFCameraModifier_FOVImpact : public UCameraModifier
{
	GENERATED_BODY()
	
public:
	UVFCameraModifier_FOVImpact();

	// 讓子系統呼叫來初始化這次衝擊的參數
	void InitFOVImpact(float InFOVOffset, float InDuration, UCurveFloat* InImpactCurve);

protected:
	// 這是 UCameraModifier 的核心，每幀都會自動呼叫來修改攝影機參數
	virtual bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;

private:
	float TargetFOVOffset;
	float ImpactDuration;
	float ElapsedTime;

	// 用來控制衝擊節奏的曲線 (例如：瞬間衝到 1.0，然後慢慢滑落回 0.0)
	UPROPERTY(Transient)
	UCurveFloat* ImpactCurve;
};
