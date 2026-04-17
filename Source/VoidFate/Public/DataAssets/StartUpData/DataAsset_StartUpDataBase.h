// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UVFAbilitySystemComponent;
class UGameplayEffect;
class UVFGameplayAbility;
/**
 * 
 */
UCLASS()
class VOIDFATE_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UVFAbilitySystemComponent* InASCToGive, int32 AbilityLevel = 1);
	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UVFGameplayAbility>> ActivateOnGivenAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UVFGameplayAbility>> ReactiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;

	void GrantAbilities(const TArray<TSubclassOf<UVFGameplayAbility>>& InAbilities, UVFAbilitySystemComponent* InASCToGive, int32 AbilityLevel = 1);
};
