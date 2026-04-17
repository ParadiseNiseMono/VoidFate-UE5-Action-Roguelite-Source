// Paradise NiseMono All Rights Reserved


#include "AbilitySystem/Abilities/VFGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "VFFunctionLibrary.h"
#include "VFGameplayTags.h"
#include "AbilitySystem/VFAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"

void UVFGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (ActivationPolicy == EVFAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UVFGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (ActivationPolicy == EVFAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UPawnCombatComponent* UVFGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UVFAbilitySystemComponent* UVFGameplayAbility::GetVFAbilitySystemComponentFromActorInfo() const
{
	return CastChecked<UVFAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UVFGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& InSpecHandle)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	check(TargetASC && InSpecHandle.IsValid());

	return GetVFAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
	 		*InSpecHandle.Data,
			TargetASC
		);
}

FActiveGameplayEffectHandle UVFGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& InSpecHandle, EVFSuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor,InSpecHandle);

	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied()? EVFSuccessType::Successful : EVFSuccessType::Failed;

	return ActiveGameplayEffectHandle;
}

void UVFGameplayAbility::ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle,
	const TArray<FHitResult>& InHitResults)
{
	if (InHitResults.IsEmpty())
	{
		return;
	}

	APawn* OwningPawn = CastChecked<APawn>(GetAvatarActorFromActorInfo());
	TArray<APawn*> AppliedPawns;

	for (const FHitResult& Hit : InHitResults)
	{
		if (APawn* HitPawn = Cast<APawn>(Hit.GetActor()))
		{
			if (AppliedPawns.Contains(HitPawn)) continue;

			bool bIsValidBlock = false;

			AppliedPawns.Add(HitPawn);

			const bool bIsPlayerBlocking = UVFFunctionLibrary::NativeDoesActorHaveTag(HitPawn, VFGameplayTags::Player_Status_Blocking);
			const bool bIsPlayerDodging = UVFFunctionLibrary::NativeDoesActorHaveTag(HitPawn, VFGameplayTags::Player_Status_PerfectDodgeWindow);
			const bool bIsMyAttackUnblockable = UVFFunctionLibrary::NativeDoesActorHaveTag(OwningPawn, VFGameplayTags::Enemy_Status_UnBlockable);

			if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
			{
				bIsValidBlock = UVFFunctionLibrary::IsValidBlock(OwningPawn, HitPawn);
			}

			if (bIsValidBlock)
			{
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
					HitPawn,
					VFGameplayTags::Player_Event_SuccessfulBlock,
					UVFFunctionLibrary::MakeGameplayEventDataWithHitResult(OwningPawn, HitPawn, Hit)
				);
			}
			else if (bIsPlayerDodging)
			{
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
					HitPawn,
					VFGameplayTags::Player_Event_PerfectDodge,
					UVFFunctionLibrary::MakeGameplayEventDataWithHitResult(OwningPawn, HitPawn, Hit)
				);
			}
			else if (UVFFunctionLibrary::IsTargetPawnHostile(OwningPawn, HitPawn))
			{
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(HitPawn,InSpecHandle);

				if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
				{	
					FGameplayEventData Data;
					Data.Instigator = OwningPawn;
					Data.Target = HitPawn;
					    
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
						HitPawn,
						VFGameplayTags::Shared_Event_HitReact,
						Data
					);
				}
			}
		}
	}
}
