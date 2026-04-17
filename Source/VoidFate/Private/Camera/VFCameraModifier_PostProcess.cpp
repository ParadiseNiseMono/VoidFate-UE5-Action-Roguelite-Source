// Paradise NiseMono All Rights Reserved

#include "Camera/VFCameraModifier_PostProcess.h"

UVFCameraModifier_PostProcess::UVFCameraModifier_PostProcess()
{
	Priority = 100;
	ElapsedTime = 0.0f;
}

void UVFCameraModifier_PostProcess::InitPostProcessFlash(UMaterialInterface* InMaterial, UCurveFloat* InCurve)
{
	if(InMaterial)
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(InMaterial, this);
	}
	EffectCurve = InCurve;
	ElapsedTime = 0.0f;

	EnableModifier();
}

void UVFCameraModifier_PostProcess::InitPersistentPostProcess(UMaterialInterface* InMaterial)
{
	if (InMaterial)
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(InMaterial, this);

		DynamicMaterialInstance->SetScalarParameterValue(FName("EffectStrength"), 1.0f);
	}

	EffectCurve = nullptr;
	ElapsedTime = 0.0f;
	EnableModifier();
}

bool UVFCameraModifier_PostProcess::ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV)
{
	if (!DynamicMaterialInstance) return false;

    float Strength = 1.0f;

    if (EffectCurve)
    {
        float TimeMin, TimeMax;
        EffectCurve->GetTimeRange(TimeMin, TimeMax);

        if (ElapsedTime > TimeMax)
        {
            DisableModifier(true);
            return false;
        }

        Strength = EffectCurve->GetFloatValue(ElapsedTime);
        DynamicMaterialInstance->SetScalarParameterValue(FName("EffectStrength"), Strength);
        ElapsedTime += DeltaTime;
    }

    if (Strength > 0.0f)
    {
        FWeightedBlendable Blendable;
        Blendable.Object = DynamicMaterialInstance;
        Blendable.Weight = 1.0f; 

        InOutPOV.PostProcessSettings.WeightedBlendables.Array.Add(Blendable);
    }

    return false;
}

