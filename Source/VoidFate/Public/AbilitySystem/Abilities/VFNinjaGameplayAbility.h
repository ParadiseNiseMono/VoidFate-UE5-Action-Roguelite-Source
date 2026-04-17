// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/VFGameplayAbility.h"
#include "VFNinjaGameplayAbility.generated.h"

class UNinjaUIComponent;
class UNinjaCombatComponent;
class AVFNinjaController;
class AVFNinjaCharacter;
/**
 * 玩家所有能力的父親
 */
UCLASS()
class VOIDFATE_API UVFNinjaGameplayAbility : public UVFGameplayAbility
{
	GENERATED_BODY()

public:
	UVFNinjaGameplayAbility();

	UFUNCTION(BlueprintPure, Category = "VoidFate|Ability")
	AVFNinjaCharacter* GetNinjaCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "VoidFate|Ability")
	AVFNinjaController* GetNinjaControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "VoidFate|Ability")
	UNinjaCombatComponent* GetNinjaCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "VoidFate|Ability")
	UNinjaUIComponent* GetNinjaUIComponentFromActorInfo();

	UFUNCTION(BlueprintCallable, Category = "VoidFate|Ability")
	void UpdateTargetWarping(float InSearchRadius = 200.0f, float InSearchAngle = 180.0f, FName InWarpTargetName = FName("NearestTarget"));

	UFUNCTION(BlueprintPure, Category = "VoidFate|Ability")
	FGameplayEffectSpecHandle MakeNinjaDamageEffectSpecHandle(
		TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InCurrentComboCount);

	//If RemainingCooldownTime > 0.f return true
	UFUNCTION(BlueprintCallable, Category = "VoidFate|Ability")
	bool GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag,float& TotalCooldownTime,float& RemainingCooldownTime);
private:
	TWeakObjectPtr<AVFNinjaCharacter> CachedNinjaCharacter;

	TWeakObjectPtr<AVFNinjaController> CachedNinjaController;
};
