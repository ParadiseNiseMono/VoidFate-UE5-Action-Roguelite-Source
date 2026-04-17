// Paradise NiseMono All Rights Reserved


#include "AbilitySystem/GEExacCalc/GEExecCalc_DamageTaken.h"

#include "VFDebugHelper.h"
#include "VFGameplayTags.h"
#include "AbilitySystem/VFAttributeSet.h"

struct FVFDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(IncomingDamage);

	FVFDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UVFAttributeSet,AttackPower,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UVFAttributeSet,Defense,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UVFAttributeSet,IncomingDamage,Target,false);
	}
};

static const FVFDamageCapture& GetVFDamageCapture()
{
	static FVFDamageCapture WarriorDamageCapture;
	return WarriorDamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	RelevantAttributesToCapture.Add(GetVFDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetVFDamageCapture().DefenseDef);
	RelevantAttributesToCapture.Add(GetVFDamageCapture().IncomingDamageDef);
}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetVFDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);
	/*Debug::Print(TEXT("SourceAttackPower"),SourceAttackPower);*/

	float BaseDamage = 0.f;
	int32 UsedLightAttackComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;

	for (const TTuple<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(VFGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
			/*Debug::Print(TEXT("BaseDamage"),BaseDamage);*/
		}
		else if (TagMagnitude.Key.MatchesTagExact(VFGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttackComboCount = TagMagnitude.Value;
			/*Debug::Print(TEXT("UsedLightAttackComboCount"),(float)UsedLightAttackComboCount);*/
		}
		else if (TagMagnitude.Key.MatchesTagExact(VFGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UsedHeavyAttackComboCount = TagMagnitude.Value;
			/*Debug::Print(TEXT("UsedHeavyAttackComboCount"),(float)UsedHeavyAttackComboCount);*/
		}
	}

	float TargetDefense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetVFDamageCapture().DefenseDef, EvaluateParameters, TargetDefense);

	/*Debug::Print(TEXT("TargetDefense"),TargetDefense);*/

	if (UsedLightAttackComboCount != 0)
	{
		const float DamageIncreasePercentLight = (UsedLightAttackComboCount - 1) * 0.05 + 1.f;

		BaseDamage *= DamageIncreasePercentLight;
		/*Debug::Print(TEXT("ScaledBaseDamageLight"),BaseDamage);*/
	}

	if (UsedHeavyAttackComboCount != 0)
	{
		const float DamageIncreasePercentHeavy = UsedHeavyAttackComboCount * 0.15f + 1.f;

		BaseDamage *= DamageIncreasePercentHeavy;
		/*Debug::Print(TEXT("ScaledBaseDamageHeavy"),BaseDamage);*/
	}

	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefense;
	/*Debug::Print(TEXT("FinalDamageDone"),FinalDamageDone);*/

	if (FinalDamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetVFDamageCapture().IncomingDamageProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			)
		);
	}
}
