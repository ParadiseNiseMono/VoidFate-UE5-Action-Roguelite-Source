// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "VFTypes/VFStructTypes.h"
#include "DataAsset_NinjaStartUpData.generated.h"

class UVFNinjaGameplayAbility;

/**
 * 
 */
UCLASS()
class VOIDFATE_API UDataAsset_NinjaStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UVFAbilitySystemComponent* InASCToGive, int32 AbilityLevel = 1) override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta=(TitleProperty = "InputTag"))
	TArray<FVFNinjaAbilitySet> NinjaStartUpAbilitySets;
};
