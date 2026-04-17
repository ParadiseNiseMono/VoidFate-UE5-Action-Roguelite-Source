// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/PawnExtensionComponentBase.h"
#include "PawnCombatComponent.generated.h"

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand,
	LeftFoot,
	RightFoot
};

class AVFWeaponBase;
/**
 * 
 */
UCLASS()
class VOIDFATE_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "VoidFate|Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AVFWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

	UFUNCTION(BlueprintCallable, Category = "VoidFate|Combat")
	AVFWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	UPROPERTY(BlueprintReadWrite, Category = "VoidFate|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable, Category = "VoidFate|Combat")
	AVFWeaponBase* GetCharacterCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "VoidFate|Combat")
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType);

	virtual void OnHitTargetActor(AActor* HitActor, const FHitResult& ImpactResult);
	virtual void OnWeaponPulledFromTargetActor(AActor* PulledActor, const FHitResult& ImpactResult);

protected:
	virtual void ToggleCurrentEquippedWeaponCollision(bool bShouldEnable);
	virtual void ToggleBodyCollisionBoxCollision(bool bShouldEnable,EToggleDamageType ToggleDamageType);
	
	TArray<AActor*> OverlappedActors;
private:
	TMap<FGameplayTag, AVFWeaponBase*> CharacterCarriedWeaponMap;
};
