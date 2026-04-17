// Paradise NiseMono All Rights Reserved


#include "Components/Combat/EnemyCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "VFDebugHelper.h"
#include "VFFunctionLibrary.h"
#include "VFGameplayTags.h"
#include "Abilities/GameplayAbilityTypes.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor, const FHitResult& ImpactResult)
{
	if (OverlappedActors.Contains(HitActor)) return;

	OverlappedActors.AddUnique(HitActor);
	
	bool bIsValidBlock = false;
	
	const bool bIsPlayerBlocking = UVFFunctionLibrary::NativeDoesActorHaveTag(HitActor, VFGameplayTags::Player_Status_Blocking);
	const bool bIsPlayerDodging = UVFFunctionLibrary::NativeDoesActorHaveTag(HitActor, VFGameplayTags::Player_Status_PerfectDodgeWindow);
	const bool bIsMyAttackUnblockable = UVFFunctionLibrary::NativeDoesActorHaveTag(GetOwningPawn(), VFGameplayTags::Enemy_Status_UnBlockable);

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		bIsValidBlock = UVFFunctionLibrary::IsValidBlock(GetOwner(), HitActor);
	}

	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitActor,
			VFGameplayTags::Player_Event_SuccessfulBlock,
			UVFFunctionLibrary::MakeGameplayEventDataWithHitResult(GetOwningPawn(), HitActor, ImpactResult)
		);
	}
	else if (bIsPlayerDodging)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitActor,
			VFGameplayTags::Player_Event_PerfectDodge,
			UVFFunctionLibrary::MakeGameplayEventDataWithHitResult(GetOwningPawn(), HitActor, ImpactResult)
		);
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			VFGameplayTags::Shared_Event_MeleeHit,
			UVFFunctionLibrary::MakeGameplayEventDataWithHitResult(GetOwningPawn(), HitActor, ImpactResult)
		);
	}
}
