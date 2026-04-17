// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "GameplayTagContainer.h"
#include "NinjaUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChangedDelegate, TSoftObjectPtr<UTexture2D>, SoftWeaponIcon);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityIconSlotUpdatedDelegate, FGameplayTag,AbilityInputTag, TSoftObjectPtr<UMaterialInterface>, SoftAbilityIconMaterial);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAbilityCooldownBeginDelegate, FGameplayTag, AbilityInputTag, float, TotalCooldownTime, float, RemainingCooldownTime);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemInteractedDelegate, bool, bShouldDisplayInputKey);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCooldownCanceledDelegate, FGameplayTag, AbilityInputTag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatusTagGrantedDelegate, FGameplayTag, AbilityInputTag, int32, NewCount);
/**
 * 
 */
UCLASS()
class VOIDFATE_API UNinjaUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentStaminaChanged;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnEquippedWeaponChangedDelegate OnEquippedWeaponChanged;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAbilityIconSlotUpdatedDelegate OnAbilityIconSlotUpdated;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAbilityCooldownBeginDelegate OnAbilityCooldownBegin;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemInteractedDelegate OnItemInteracted;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnCooldownCanceledDelegate OnCooldownCanceled;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAbilityCooldownBeginDelegate OnAbilityCooldownUpdated;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnStatusTagGrantedDelegate OnStatusTagGranted;
};
