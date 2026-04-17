// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/VFWeaponBase.h"
#include "VFTypes/VFStructTypes.h"
#include "VFNinjaWeapon.generated.h"

struct FGameplayAbilitySpecHandle;
/**
 * 
 */
UCLASS()
class VOIDFATE_API AVFNinjaWeapon : public AVFWeaponBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FVFNinjaWeaponData NinjaWeaponData;

	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;

private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};
