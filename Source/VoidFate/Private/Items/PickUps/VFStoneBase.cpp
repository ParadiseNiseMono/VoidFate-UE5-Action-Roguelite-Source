// Paradise NiseMono All Rights Reserved


#include "Items/PickUps/VFStoneBase.h"

#include "VFGameplayTags.h"
#include "AbilitySystem/VFAbilitySystemComponent.h"
#include "Characters/VFNinjaCharacter.h"

void AVFStoneBase::PickUpStones(UVFAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	if (StoneGameplayEffectClass)
	{
		UGameplayEffect* EffectCDO = StoneGameplayEffectClass->GetDefaultObject<UGameplayEffect>();
	
		AbilitySystemComponent->ApplyGameplayEffectToSelf(
			EffectCDO,
			ApplyLevel,
			AbilitySystemComponent->MakeEffectContext()
		);
	}

	BP_OnStonePickUped();
}

void AVFStoneBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AVFNinjaCharacter* OverlappedNinjaCharacter = Cast<AVFNinjaCharacter>(OtherActor))
	{
		OverlappedNinjaCharacter->GetVFAbilitySystemComponent()->TryActivateAbilityByTag(VFGameplayTags::Player_PickUp_Stones);
	}
}
