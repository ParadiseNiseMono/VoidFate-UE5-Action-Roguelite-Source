// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "VFCameraModifier_PostProcess.generated.h"

class UCurveFloat;
class UMaterialInstanceDynamic;

/**
 * 專門負責管理「暫時性」後製特效的修改器
 * 例如：處決時的色彩抽離、受傷時的血屏
 */
UCLASS()
class VOIDFATE_API UVFCameraModifier_PostProcess : public UCameraModifier
{
	GENERATED_BODY()
	
public:
	UVFCameraModifier_PostProcess();

	// 初始化特效參數
	void InitPostProcessFlash(UMaterialInterface* InMaterial, UCurveFloat* InCurve);

	void InitPersistentPostProcess(UMaterialInterface* InMaterial);

protected:
	virtual bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;

private:
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	UPROPERTY(Transient)
	UCurveFloat* EffectCurve;

	float ElapsedTime;
};
