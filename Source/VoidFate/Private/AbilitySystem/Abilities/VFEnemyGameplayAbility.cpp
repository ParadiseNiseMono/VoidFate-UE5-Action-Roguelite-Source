// Paradise NiseMono All Rights Reserved


#include "AbilitySystem/Abilities/VFEnemyGameplayAbility.h"

#include "VFGameplayTags.h"
#include "AbilitySystem/VFAbilitySystemComponent.h"
#include "Characters/VFEnemyCharacter.h"

UVFEnemyGameplayAbility::UVFEnemyGameplayAbility()
{
	FGameplayTag EntryPlayAnimationTag = VFGameplayTags::Shared_Status_Entry_PlayAnimation;

	ActivationBlockedTags.AddTag(EntryPlayAnimationTag);
}

AVFEnemyCharacter* UVFEnemyGameplayAbility::GetVFEnemyCharacterFromActorInfo()
{
	if (!CachedEnemyCharacter.IsValid())
	{
		CachedEnemyCharacter = CastChecked<AVFEnemyCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedEnemyCharacter.IsValid() ? CachedEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UVFEnemyGameplayAbility::GetEnemyCombatComponentFormActorInfo()
{
	return GetVFEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}

FGameplayEffectSpecHandle UVFEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(
	TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
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
		InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel())
	);

	return EffectSpecHandle;
}
