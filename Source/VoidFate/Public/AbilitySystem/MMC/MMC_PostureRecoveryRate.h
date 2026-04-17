// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_PostureRecoveryRate.generated.h"

/**
 * 
 */
UCLASS()
class VOIDFATE_API UMMC_PostureRecoveryRate : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_PostureRecoveryRate();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition HealthDef;
	FGameplayEffectAttributeCaptureDefinition MaxHealthDef;
	FGameplayEffectAttributeCaptureDefinition PostureRecoveryDef;
};
