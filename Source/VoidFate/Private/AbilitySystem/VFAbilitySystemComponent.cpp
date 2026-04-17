// Paradise NiseMono All Rights Reserved


#include "AbilitySystem/VFAbilitySystemComponent.h"

#include "VFGameplayTags.h"
#include "AbilitySystem/Abilities/VFNinjaGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "VFFunctionLibrary.h"

void UVFAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	TArray<FGameplayAbilitySpecHandle> HandlesToActivate;
	TArray<FGameplayAbilitySpecHandle> HandlesToCancel;

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag)) continue;

		if (InInputTag.MatchesTag(VFGameplayTags::Input_Toggleable) && AbilitySpec.IsActive() 
			&& !UVFFunctionLibrary::NativeDoesActorHaveTag(GetAvatarActor(), VFGameplayTags::Shared_Status_BlockCancelAbility))
		{
			HandlesToCancel.Add(AbilitySpec.Handle);
		}
		else
		{
			HandlesToActivate.Add(AbilitySpec.Handle);
		}
	}

	for (const FGameplayAbilitySpecHandle& Handle : HandlesToCancel)
	{
		CancelAbilityHandle(Handle);
	}

	for (const FGameplayAbilitySpecHandle& Handle : HandlesToActivate)
	{
		TryActivateAbility(Handle);
	}
}

void UVFAbilitySystemComponent::OnAbilityInputHeld(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid() || !InInputTag.MatchesTag(VFGameplayTags::Input_MustHold)) return;

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag)) continue;

		if (!AbilitySpec.IsActive())
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UVFAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid() || !InInputTag.MatchesTag(VFGameplayTags::Input_MustHold)) return;

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{  
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag) && AbilitySpec.IsActive())
		{
			CancelAbilityHandle(AbilitySpec.Handle);
		}
	}
}

void UVFAbilitySystemComponent::GrantNinjaWeaponAbilities(const TArray<FVFNinjaAbilitySet>& InDefaultWeaponAbilities,
	const TArray<FVFNinjaSpecialAbilitySet>& InSpecialWeaponAbilities,
	int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty()) return;

	for (const FVFNinjaAbilitySet& AbilitySet: InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);

		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
	for (const FVFNinjaSpecialAbilitySet& AbilitySet: InSpecialWeaponAbilities)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);

		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

void UVFAbilitySystemComponent::RemoveGrantedNinjaWeaponAbilities(
	UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	if (InSpecHandlesToRemove.IsEmpty()) return;

	for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
	{
		if (SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}

	InSpecHandlesToRemove.Empty();
}

bool UVFAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
	check(AbilityTagToActivate.IsValid());

	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpecs);

	if (!FoundAbilitySpecs.IsEmpty())
	{
		int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
		FGameplayAbilitySpec* AbilitySpec = FoundAbilitySpecs[RandomAbilityIndex];

		check(AbilitySpec);

		if (!AbilitySpec->IsActive())
		{
			TryActivateAbility(AbilitySpec->Handle);
		}
	}
	return false;
}

bool UVFAbilitySystemComponent::TryCancelAbilityByTag(FGameplayTag AbilityTagToCancel)
{
	check(AbilityTagToCancel.IsValid());
	
	FGameplayTagContainer CancelTags(AbilityTagToCancel);
	
	CancelAbilities(&CancelTags, nullptr, nullptr);
	
	return true;
}

bool UVFAbilitySystemComponent::TryActivateAbilityByEvent(FGameplayTag EventTag, AActor* TargetActor)
{
	if (!EventTag.IsValid() || !TargetActor)
	{
		return false;
	}

	FGameplayEventData Payload;
	Payload.EventTag = EventTag;
	Payload.Instigator = GetAvatarActor();
	Payload.Target = TargetActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetAvatarActor(),
		EventTag,
		Payload
	);

	return true;
}

void UVFAbilitySystemComponent::ReduceActiveCooldownByTags(FGameplayTagContainer CooldownTags, float TimeToReduce)
{
	FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
	TArray<FActiveGameplayEffectHandle> ActiveEffects = GetActiveEffects(Query);

	for (const FActiveGameplayEffectHandle& Handle : ActiveEffects)
	{
		const FActiveGameplayEffect* ActiveEffect = GetActiveGameplayEffect(Handle);

		if (!ActiveEffect || ActiveEffect->GetDuration() <= 0.0f) continue;

		ActiveGameplayEffects.ModifyActiveEffectStartTime(Handle, -TimeToReduce);

		float ExpectedEndTime = ActiveEffect->StartServerWorldTime + ActiveEffect->GetDuration();
		float CurrentTime = GetWorld()->GetTimeSeconds();

		if (CurrentTime >= ExpectedEndTime)
		{
			RemoveActiveGameplayEffect(Handle);
		}
	}
}
