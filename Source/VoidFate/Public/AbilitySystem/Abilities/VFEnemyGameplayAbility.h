// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/VFGameplayAbility.h"
#include "VFEnemyGameplayAbility.generated.h"

class UEnemyCombatComponent;
class AVFEnemyCharacter;
/**
 * 敵人所有能力的父親
 */
UCLASS()
class VOIDFATE_API UVFEnemyGameplayAbility : public UVFGameplayAbility
{
	GENERATED_BODY()

public:
	UVFEnemyGameplayAbility();

	UFUNCTION(BlueprintPure, Category="VoidFate|Ability")
	AVFEnemyCharacter* GetVFEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category="VoidFate|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFormActorInfo();

	UFUNCTION(BlueprintPure, Category="VoidFate|Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass,const FScalableFloat& InDamageScalableFloat);
	
private:
	TWeakObjectPtr<AVFEnemyCharacter> CachedEnemyCharacter;
};
