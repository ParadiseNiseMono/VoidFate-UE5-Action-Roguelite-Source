// Paradise NiseMono All Rights Reserved


#include "DataAssets/StartUpData/DataAsset_NinjaStartUpData.h"

#include "AbilitySystem/VFAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/VFNinjaGameplayAbility.h"

void UDataAsset_NinjaStartUpData::GiveToAbilitySystemComponent(UVFAbilitySystemComponent* InASCToGive,
	int32 AbilityLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, AbilityLevel);

	for (const FVFNinjaAbilitySet& AbilitySet : NinjaStartUpAbilitySets)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.Level = AbilityLevel;
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);
		
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
