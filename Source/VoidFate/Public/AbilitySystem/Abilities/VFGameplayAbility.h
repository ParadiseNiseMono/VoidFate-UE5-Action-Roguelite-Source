// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "VFTypes/VFEnumTypes.h"
#include "VFGameplayAbility.generated.h"

class UVFAbilitySystemComponent;
class UPawnCombatComponent;

UENUM(BlueprintType)
enum class EVFAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};

/**
 * 
 */
UCLASS()
class VOIDFATE_API UVFGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface
	
	UPROPERTY(EditDefaultsOnly, Category = "VoidFate|Ability")
	EVFAbilityActivationPolicy ActivationPolicy = EVFAbilityActivationPolicy::OnTriggered;

	UFUNCTION(BlueprintPure, Category = "VoidFate|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "VoidFate|Ability")
	UVFAbilitySystemComponent* GetVFAbilitySystemComponentFromActorInfo() const;

	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor,const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "VoidFate|Ability", meta = (DisplayName = "Apply Effect Spec Handle To Target", ExpandEnumAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor,const FGameplayEffectSpecHandle& InSpecHandle,EVFSuccessType& OutSuccessType);

	UFUNCTION(BlueprintCallable, Category = "VoidFate|Ability")
	void ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle,const TArray<FHitResult>& InHitResults);
};
