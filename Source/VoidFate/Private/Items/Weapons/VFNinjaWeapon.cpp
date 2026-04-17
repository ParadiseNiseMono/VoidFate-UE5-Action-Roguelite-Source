// Paradise NiseMono All Rights Reserved


#include "Items/Weapons/VFNinjaWeapon.h"
#include "GameplayAbilitySpecHandle.h"

void AVFNinjaWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	GrantedAbilitySpecHandles = InSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> AVFNinjaWeapon::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitySpecHandles;
}
