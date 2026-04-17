// Paradise NiseMono All Rights Reserved

#include "VFTypes/VFStructTypes.h"

#include "AbilitySystem/Abilities/VFNinjaGameplayAbility.h"


bool FVFNinjaAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
