// Paradise NiseMono All Rights Reserved


#include "Components/Combat/NinjaCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "VFDebugHelper.h"
#include "VFFunctionLibrary.h"
#include "VFGameplayTags.h"
#include "Items/Weapons/VFNinjaWeapon.h"
#include "Subsystems/VFCombatFeedbackSubsystem.h"

AVFNinjaWeapon* UNinjaCombatComponent::GetNinjaCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<AVFNinjaWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

AVFNinjaWeapon* UNinjaCombatComponent::GetNinjaCurrentEquippedWeapon() const
{
	if (!GetCharacterCurrentEquippedWeapon())
	{
		UE_LOG(LogTemp, Warning, TEXT("UNinjaCombatComponent::GetNinjaCurrentEquippedWeapon: No current equipped weapon found!"));
		return nullptr;
	}
	return Cast<AVFNinjaWeapon>(GetCharacterCurrentEquippedWeapon());
}

float UNinjaCombatComponent::GetNinjaCurrentEquippedWeaponDamageAtLevel(int32 InLevel) const
{
	if (!GetNinjaCurrentEquippedWeapon()) {
		UE_LOG(LogTemp, Warning, TEXT("UNinjaCombatComponent::GetNinjaCurrentEquippedWeapon: No current equipped weapon found!"));
		return 0;
	}
	return GetNinjaCurrentEquippedWeapon()->NinjaWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}

void UNinjaCombatComponent::OnHitTargetActor(AActor* HitActor, const FHitResult& ImpactResult)
{
	if (OverlappedActors.Contains(HitActor)) return;

	OverlappedActors.AddUnique(HitActor);
	//UE_LOG(LogTemp, Warning, TEXT("After Receive: Location = %s"), *ImpactResult.ImpactPoint.ToString());

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		VFGameplayTags::Shared_Event_MeleeHit,
		UVFFunctionLibrary::MakeGameplayEventDataWithHitResult(GetOwningPawn(), HitActor, ImpactResult)
	);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		VFGameplayTags::Player_Event_HitPause,
		UVFFunctionLibrary::MakeGameplayEventDataWithHitResult(GetOwningPawn(), HitActor, ImpactResult)
	);
}

void UNinjaCombatComponent::OnWeaponPulledFromTargetActor(AActor* PulledActor, const FHitResult& ImpactResult)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		VFGameplayTags::Player_Event_HitPause,
		FGameplayEventData()
	);
}
