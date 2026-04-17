// Paradise NiseMono All Rights Reserved


#include "AbilitySystem/VFAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "VFDebugHelper.h"
#include "VFFunctionLibrary.h"
#include "VFGameplayTags.h"
#include "Components/UI/NinjaUIComponent.h"
#include "Components/UI/PawnUIComponent.h"
#include "Interfaces/PawnUIInterface.h"
#include "AbilitySystemBlueprintLibrary.h"

UVFAttributeSet::UVFAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitMaxStamina(1.f);
	InitCurrentStamina(1.f);
	InitCurrentPosture(1.f);
	InitPostureRecovery(1.f);
	InitMaxPosture(1.f);
	InitPostureDamage(1.f);
	InitAttackPower(1.f);
	InitDodgeCost(1.f);
	InitDefense(1.f);
}

void UVFAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UVFAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UVFAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (!CachedPawnUIInterface.IsValid())
	{
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}
	checkf(CachedPawnUIInterface.IsValid(), TEXT("%s didn't implement IPawnUIInterface"),*Data.Target.GetAvatarActor()->GetActorNameOrLabel());

	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();

	checkf(PawnUIComponent, TEXT("Couldn't extract a PawnUIComponent from %s"),*Data.Target.GetAvatarActor()->GetActorNameOrLabel());
	
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth(), GetMaxHealth());
	}
	if (Data.EvaluatedData.Attribute == GetCurrentStaminaAttribute())
	{
		const float NewCurrentStamina = FMath::Clamp(GetCurrentStamina(), 0.f, GetMaxStamina());

		SetCurrentStamina(NewCurrentStamina);

		if (GetCurrentStamina() == GetMaxStamina())
		{
			UVFFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), VFGameplayTags::Player_Status_Stamina_Full);
		}
		else if (GetCurrentStamina() == 0.f)
		{
			UVFFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), VFGameplayTags::Player_Status_Stamina_None);
		}
		else
		{
			UVFFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), VFGameplayTags::Player_Status_Stamina_Full);
			UVFFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), VFGameplayTags::Player_Status_Stamina_None);
		}

		if (UNinjaUIComponent* NinjaUIComponent = CachedPawnUIInterface->GetNinjaUIComponent())
		{
			NinjaUIComponent->OnCurrentStaminaChanged.Broadcast(NewCurrentStamina, GetMaxStamina());
		}
	}
	if (Data.EvaluatedData.Attribute == GetCurrentPostureAttribute())
	{
		const float NewPosture = FMath::Clamp(GetCurrentPosture(), 0.f, GetMaxPosture());

		SetCurrentPosture(NewPosture);
		
		if (GetCurrentPosture() == GetMaxPosture())
		{
			HandlePostureBreak(GetOwningAbilitySystemComponent());
			UVFFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), VFGameplayTags::Shared_Status_Posture_Full);
		}
		else if (GetCurrentPosture() == 0.f)
		{
			UVFFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), VFGameplayTags::Shared_Status_Posture_None);
		}
		else
		{
			UVFFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), VFGameplayTags::Shared_Status_Posture_Full);
			UVFFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), VFGameplayTags::Shared_Status_Posture_None);
		}

		PawnUIComponent->OnCurrentPostureChanged.Broadcast(GetCurrentPosture(), GetMaxPosture());
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float IncomingDamageDone = GetIncomingDamage();

		const float NewHealth = FMath::Clamp(OldHealth - IncomingDamageDone, 0.f, GetMaxHealth());

		SetCurrentHealth(NewHealth);

		/*const FString DebugString = FString::Printf(
			TEXT("Old Health: %f, Damage Done: %f, NewCurrentHealth: %f"),
			OldHealth,
			IncomingDamageDone,
			NewHealth
		);*/

		/*const FString DebugString = FString::Printf(
			TEXT("Posture Damage: %f, New Posture: %f"),
			PostureDamage,
			NewPosture
		);

		Debug::Print(DebugString);*/

		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth(), GetMaxHealth());
		
		if (GetCurrentHealth() == 0.f)
		{
			bool bIsAlreadyDead = UVFFunctionLibrary::NativeDoesActorHaveTag(Data.Target.GetAvatarActor(), VFGameplayTags::Shared_Status_Dead);
			if (!bIsAlreadyDead) {
				UVFFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), VFGameplayTags::Shared_Status_Dead);
				AActor* Instigator = Data.EffectSpec.GetContext().GetInstigator();

				FGameplayEventData EventData;
				EventData.Instigator = Instigator;
				EventData.Target = Data.Target.GetAvatarActor();

				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Instigator, VFGameplayTags::Player_Event_KilledEnemy, EventData);
				SetCurrentPosture(0.f);
			}
		}
		
	}
}

void UVFAttributeSet::HandlePostureBreak(UAbilitySystemComponent* TargetASC)
{
	if (!TargetASC) return;

	UVFFunctionLibrary::AddGameplayTagToActorIfNone(TargetASC->GetAvatarActor(), VFGameplayTags::Shared_Status_Stunned);
}
