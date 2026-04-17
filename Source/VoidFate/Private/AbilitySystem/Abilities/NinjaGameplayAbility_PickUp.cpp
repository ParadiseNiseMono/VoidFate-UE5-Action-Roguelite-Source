// Paradise NiseMono All Rights Reserved


#include "AbilitySystem/Abilities/NinjaGameplayAbility_PickUp.h"

#include "Characters/VFNinjaCharacter.h"
#include "Components/UI/NinjaUIComponent.h"
#include "Items/PickUps/VFStoneBase.h"
#include "Kismet/KismetSystemLibrary.h"

void UNinjaGameplayAbility_PickUp::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	GetNinjaUIComponentFromActorInfo()->OnItemInteracted.Broadcast(true);
}

void UNinjaGameplayAbility_PickUp::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetNinjaUIComponentFromActorInfo()->OnItemInteracted.Broadcast(false);
}

void UNinjaGameplayAbility_PickUp::CollectStones()
{
	CollectedStones.Empty();

	TArray<FHitResult> TraceHits;

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetNinjaCharacterFromActorInfo(),
		GetNinjaCharacterFromActorInfo()->GetActorLocation(),
		GetNinjaCharacterFromActorInfo()->GetActorLocation() + -GetNinjaCharacterFromActorInfo()->GetActorUpVector() * BoxTraceDistance,
		TraceBoxSize / 2.f,
		(-GetNinjaCharacterFromActorInfo()->GetActorUpVector()).ToOrientationRotator(),
		StoneTraceChannel,
		false,
		TArray<AActor*>(),
		bDrawDebugShape? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		TraceHits,
		true
	);

	for (const FHitResult& TraceHit : TraceHits)
	{
		if (AVFStoneBase* FoundStone = Cast<AVFStoneBase>(TraceHit.GetActor()))
		{
			CollectedStones.AddUnique(FoundStone);
		}
	}

	if (CollectedStones.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}

void UNinjaGameplayAbility_PickUp::PickUpStones()
{
	if (CollectedStones.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
		return;
	}

	if (AVFStoneBase * CollectedStone = CollectedStones[0]) {
		CollectedStone->PickUpStones(GetVFAbilitySystemComponentFromActorInfo(), GetAbilityLevel());
	}

	/*for (AVFStoneBase* CollectedStone : CollectedStones)
	{
		if (CollectedStone)
		{
			CollectedStone->PickUpStones(GetVFAbilitySystemComponentFromActorInfo(),GetAbilityLevel());
		}
	}*/
}

