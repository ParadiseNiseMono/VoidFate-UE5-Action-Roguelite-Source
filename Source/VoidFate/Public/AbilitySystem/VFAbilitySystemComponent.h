// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "VFTypes/VFStructTypes.h"
#include "VFAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class VOIDFATE_API UVFAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputHeld(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	UFUNCTION(BlueprintCallable, Category = "VoidFate|Ability", meta = (ApplyLevel = "1"))
	void GrantNinjaWeaponAbilities(const TArray<FVFNinjaAbilitySet>& InDefaultWeaponAbilities, const TArray<FVFNinjaSpecialAbilitySet>& InSpecialWeaponAbilities,int32 ApplyLevel,TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintCallable, Category = "VoidFate|Ability")
	void RemoveGrantedNinjaWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category = "VoidFate|Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);

	UFUNCTION(BlueprintCallable, Category = "VoidFate|Ability")
	bool TryCancelAbilityByTag(FGameplayTag AbilityTagToCancel);

	// 透過 Gameplay Event 啟動技能並傳入目標
	UFUNCTION(BlueprintCallable, Category = "VoidFate|Ability")
	bool TryActivateAbilityByEvent(FGameplayTag EventTag, AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "VoidFate|GAS")
	void ReduceActiveCooldownByTags(FGameplayTagContainer CooldownTags, float TimeToReduce);
};
