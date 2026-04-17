// Paradise NiseMono All Rights Reserved


#include "Widgets/Rewards/Widget_RewardScreen.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Controllers/VFNinjaController.h"
#include "Controllers/VFUIController.h"
#include "DataAssets/Reward/PrimaryDataAsset_Reward.h"
#include "Subsystems/VFRewardSubsystem.h"
#include "Widgets/Rewards/CommonButton_RewardCard.h"

void UWidget_RewardScreen::BindAndAddRewardCard(UCommonButton_RewardCard* CardToAdd)
{
	if (CardToAdd)
	{
		CardToAdd->OnRewardCardClicked.AddUniqueDynamic(this, &UWidget_RewardScreen::HandleRewardSelected);
		
		RewardCards.AddUnique(CardToAdd);
	}
}

void UWidget_RewardScreen::HandleRewardSelected(UPrimaryDataAsset_Reward* SelectedReward)
{
	if (!SelectedReward) return;
	
	AVFNinjaController* NinjaController = GetOwningVFNanjaController();
	if (!NinjaController) return;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(NinjaController->GetPawn());

	if (ASC)
	{
		for (const TSoftClassPtr<UGameplayEffect>& EffectSoftClass : SelectedReward->EffectsToGrant)
		{
			if (UClass* EffectClass = EffectSoftClass.LoadSynchronous())
			{
				FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
				EffectContext.AddInstigator(NinjaController->GetPawn(), NinjaController->GetPawn());

				FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, 1.0f, EffectContext);
				if (SpecHandle.IsValid())
				{
					ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
		}
		
		for (const TSoftClassPtr<UGameplayAbility>& AbilitySoftClass : SelectedReward->AbilitiesToGrant)
		{
			if (UClass* AbilityClass = AbilitySoftClass.LoadSynchronous())
			{
				FGameplayAbilitySpec NewAbilitySpec(AbilityClass, 1);
				ASC->GiveAbility(NewAbilitySpec);
			}
		}
	}
	UVFRewardSubsystem::Get(GetOwningVFNanjaController())->RecordAcquiredReward(SelectedReward);
	
	DeactivateWidget();
}

TArray<UCommonButton_RewardCard*> UWidget_RewardScreen::GetRewardCards()
{
	return RewardCards;
}
