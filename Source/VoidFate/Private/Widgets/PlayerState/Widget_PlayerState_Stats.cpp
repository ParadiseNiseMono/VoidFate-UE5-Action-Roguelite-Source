// Paradise NiseMono All Rights Reserved


#include "Widgets/PlayerState/Widget_PlayerState_Stats.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CommonTextBlock.h"
#include "AbilitySystem/VFAttributeSet.h"

#define UPDATE_STAT_DISPLAY(AttributeName) \
		if (CommonTextBlock_##AttributeName) \
		{ \
		const float Value = ASC->GetNumericAttribute(UVFAttributeSet::Get##AttributeName##Attribute()); \
		CommonTextBlock_##AttributeName->SetText(FText::AsNumber(Value)); \
		}

void UWidget_PlayerState_Stats::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	APlayerController* PC = GetOwningPlayer();
	if (!PC) return;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PC->GetPawn());
	if (ASC)
	{
		UPDATE_STAT_DISPLAY(MaxHealth);
		UPDATE_STAT_DISPLAY(CurrentHealth);
		UPDATE_STAT_DISPLAY(MaxStamina);
		UPDATE_STAT_DISPLAY(CurrentStamina);
		UPDATE_STAT_DISPLAY(MaxPosture);
		UPDATE_STAT_DISPLAY(CurrentPosture);
		UPDATE_STAT_DISPLAY(PostureDamage);
		UPDATE_STAT_DISPLAY(AttackPower);
		UPDATE_STAT_DISPLAY(Defense);
	}
}
