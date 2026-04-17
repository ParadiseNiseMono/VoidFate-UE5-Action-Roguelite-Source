// Paradise NiseMono All Rights Reserved


#include "AbilitySystem/MMC/MMC_PostureRecoveryRate.h"

#include "VFDebugHelper.h"
#include "AbilitySystem/VFAttributeSet.h"
#include "DynamicMesh/DynamicMesh3.h"

UMMC_PostureRecoveryRate::UMMC_PostureRecoveryRate()
{
	HealthDef = FGameplayEffectAttributeCaptureDefinition(UVFAttributeSet::GetCurrentHealthAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	RelevantAttributesToCapture.Add(HealthDef);
	
	MaxHealthDef = FGameplayEffectAttributeCaptureDefinition(UVFAttributeSet::GetMaxHealthAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	RelevantAttributesToCapture.Add(MaxHealthDef);
	
	PostureRecoveryDef = FGameplayEffectAttributeCaptureDefinition(UVFAttributeSet::GetPostureRecoveryAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	RelevantAttributesToCapture.Add(PostureRecoveryDef);
}

float UMMC_PostureRecoveryRate::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float Health = 0.f;
	GetCapturedAttributeMagnitude(HealthDef, Spec, EvaluationParameters, Health);

	float MaxHealth = 0.f;
	GetCapturedAttributeMagnitude(MaxHealthDef, Spec, EvaluationParameters, MaxHealth);
	MaxHealth = FMath::Max<float>(MaxHealth, 1.0f); 
	
	float BaseRecovery = 0.f;
	GetCapturedAttributeMagnitude(PostureRecoveryDef, Spec, EvaluationParameters, BaseRecovery);
	
	float HealthRatio = Health / MaxHealth;
	float Multiplier = FMath::Clamp(HealthRatio, 0.1f, 1.0f);

	/*Debug::Print(FString::Printf(TEXT("PostureRecovery: %f"), -BaseRecovery * Multiplier));*/
	
	return -BaseRecovery * Multiplier;
}
