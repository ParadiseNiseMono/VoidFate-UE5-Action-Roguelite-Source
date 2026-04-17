// Paradise NiseMono All Rights Reserved


#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"

#include "AbilitySystem/VFAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/VFEnemyGameplayAbility.h"

void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UVFAbilitySystemComponent* InASCToGive,
                                                               int32 AbilityLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, AbilityLevel);

	if (EnemyCombatAbilities.IsEmpty()) return;

	for (const TSubclassOf<UVFEnemyGameplayAbility> AbilityClass : EnemyCombatAbilities)
	{
		FGameplayAbilitySpec AbilitySpec((AbilityClass.Get()));
		AbilitySpec.Level = AbilityLevel;
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
