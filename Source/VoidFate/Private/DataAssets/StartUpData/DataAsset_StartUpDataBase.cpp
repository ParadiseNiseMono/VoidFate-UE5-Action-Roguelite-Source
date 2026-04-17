// Paradise NiseMono All Rights Reserved


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"

#include "AbilitySystem/VFAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/VFGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UVFAbilitySystemComponent* InASCToGive,
                                                              int32 AbilityLevel)
{
	check(InASCToGive);

	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, AbilityLevel);
	GrantAbilities(ReactiveAbilities, InASCToGive, AbilityLevel);

	if (!StartUpGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect> EffectClass : StartUpGameplayEffects)
		{
			if (!EffectClass) continue;

			UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();
			
			InASCToGive->ApplyGameplayEffectToSelf(
				EffectCDO,
				AbilityLevel,
				InASCToGive->MakeEffectContext()
			);
		}
	}
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UVFGameplayAbility>>& InAbilities,
	UVFAbilitySystemComponent* InASCToGive, int32 AbilityLevel)
{
	if (InAbilities.IsEmpty()) return;

	for (const TSubclassOf<UVFGameplayAbility>& Ability : InAbilities)
	{
		if (!Ability || !InASCToGive) continue;

		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.Level = AbilityLevel;
		AbilitySpec.SourceObject = InASCToGive;
		
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
