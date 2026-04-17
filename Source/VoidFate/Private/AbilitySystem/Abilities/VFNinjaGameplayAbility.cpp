// Paradise NiseMono All Rights Reserved


#include "AbilitySystem/Abilities/VFNinjaGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "MotionWarpingComponent.h"
#include "VFFunctionLibrary.h"
#include "VFGameplayTags.h"
#include "AbilitySystem/VFAbilitySystemComponent.h"
#include "Characters/VFNinjaCharacter.h"
#include "Components/Combat/NinjaCombatComponent.h"
#include "Controllers/VFNinjaController.h"
#include "Kismet/KismetMathLibrary.h"

UVFNinjaGameplayAbility::UVFNinjaGameplayAbility()
{
	FGameplayTag TranscendenceActivatingTag = VFGameplayTags::Player_Status_Transcendence_Activating;

	ActivationBlockedTags.AddTag(TranscendenceActivatingTag);
}

AVFNinjaCharacter* UVFNinjaGameplayAbility::GetNinjaCharacterFromActorInfo()
{
	if (!CachedNinjaCharacter.IsValid())
	{
		CachedNinjaCharacter = Cast<AVFNinjaCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedNinjaCharacter.IsValid() ? CachedNinjaCharacter.Get() : nullptr;
}

AVFNinjaController* UVFNinjaGameplayAbility::GetNinjaControllerFromActorInfo()
{
	if (!CachedNinjaController.IsValid())
	{
		CachedNinjaController = Cast<AVFNinjaController>(CurrentActorInfo->PlayerController);
	}

	return CachedNinjaController.IsValid() ? CachedNinjaController.Get() : nullptr;
}

UNinjaCombatComponent* UVFNinjaGameplayAbility::GetNinjaCombatComponentFromActorInfo()
{
	return GetNinjaCharacterFromActorInfo()->GetNinjaCombatComponent();
}

UNinjaUIComponent* UVFNinjaGameplayAbility::GetNinjaUIComponentFromActorInfo()
{
	return GetNinjaCharacterFromActorInfo()->GetNinjaUIComponent();
}

void UVFNinjaGameplayAbility::UpdateTargetWarping(float InSearchRadius, float InSearchAngle, FName InWarpTargetName)
{
	AVFNinjaCharacter* NinjaCharacter = GetNinjaCharacterFromActorInfo();
	if (!NinjaCharacter) return;

	UMotionWarpingComponent* MotionWarpComp = NinjaCharacter->GetMotionWarpingComponent();
	if (!MotionWarpComp) return;
	
	AActor* FoundTarget = UVFFunctionLibrary::FindNearestTargetInRadius(this, NinjaCharacter, InSearchRadius, InSearchAngle);
	if (FoundTarget)
	{
		FVector TargetLoc = FoundTarget->GetActorLocation();
		FVector SelfLoc = NinjaCharacter->GetActorLocation();

		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(SelfLoc, TargetLoc);

		MotionWarpComp->AddOrUpdateWarpTargetFromLocationAndRotation(
			InWarpTargetName,
			TargetLoc,
			LookAtRot
		);
	}
	else
	{
		MotionWarpComp->RemoveWarpTarget(InWarpTargetName);
	}
}
	

FGameplayEffectSpecHandle UVFNinjaGameplayAbility::MakeNinjaDamageEffectSpecHandle(
	TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag,
	int32 InCurrentComboCount)
{
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = GetVFAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(),GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = GetVFAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		VFGameplayTags::Shared_SetByCaller_BaseDamage,
		InWeaponBaseDamage
	);

	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag,InCurrentComboCount);
	}

	return EffectSpecHandle;
}

bool UVFNinjaGameplayAbility::GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime,
	float& RemainingCooldownTime)
{
	RemainingCooldownTime = 0.f;
	TotalCooldownTime = 0.f;

	if (!InCooldownTag.IsValid())
	{
		return false;
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		return false;
	}

	FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());
	TArray< TPair<float, float> > TimeRemainingAndDuration = ASC->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);

	if (!TimeRemainingAndDuration.IsEmpty())
	{
		float HighestTime = 0.f;
		float HighestDuration = 0.f;

		for (const auto& TimePair : TimeRemainingAndDuration)
		{
			if (TimePair.Key > HighestTime)
			{
				HighestTime = TimePair.Key;
				HighestDuration = TimePair.Value;
			}
		}

		RemainingCooldownTime = HighestTime;
		TotalCooldownTime = HighestDuration;
	}

	return RemainingCooldownTime > 0.f;
}
