// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "NinjaCombatComponent.generated.h"

class AVFNinjaWeapon;
/**
 * 玩家戰鬥的邏輯處理
 */
UCLASS()
class VOIDFATE_API UNinjaCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "VoidFate|Combat")
	AVFNinjaWeapon* GetNinjaCarriedWeaponByTag(FGameplayTag InWeaponTag) const;

	UFUNCTION(BlueprintCallable, Category = "VoidFate|Combat")
	AVFNinjaWeapon* GetNinjaCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "VoidFate|Combat")
	float GetNinjaCurrentEquippedWeaponDamageAtLevel(int32 InLevel) const;

	//~ Begin UPawnCombatComponent Interface
	virtual void OnHitTargetActor(AActor* HitActor, const FHitResult& ImpactResult) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* PulledActor, const FHitResult& ImpactResult) override;
	//~ End UPawnCombatComponent Interface
};
