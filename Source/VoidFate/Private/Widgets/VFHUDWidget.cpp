 // Paradise NiseMono All Rights Reserved


#include "Widgets/VFHUDWidget.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/VFAttributeSet.h"
#include "Interfaces/PawnUIInterface.h"
#include "Widgets/VFStatBarWidget.h"

void UVFHUDWidget::InitializeHUD(UAbilitySystemComponent* ASC)
{
	if (!ASC) return;
	OwnerASC = ASC;
	
	ASC->GetGameplayAttributeValueChangeDelegate(UVFAttributeSet::GetCurrentHealthAttribute())
		.AddUObject(this, &UVFHUDWidget::OnHealthChanged);
	
	ASC->GetGameplayAttributeValueChangeDelegate(UVFAttributeSet::GetMaxHealthAttribute())
		.AddUObject(this, &UVFHUDWidget::OnHealthChanged);
	
	ASC->GetGameplayAttributeValueChangeDelegate(UVFAttributeSet::GetCurrentStaminaAttribute())
		.AddUObject(this, &UVFHUDWidget::OnStaminaChanged);
	
	ASC->GetGameplayAttributeValueChangeDelegate(UVFAttributeSet::GetCurrentPostureAttribute())
		.AddUObject(this, &UVFHUDWidget::OnPostureChanged);
	
	float CurrentHealth = ASC->GetNumericAttribute(UVFAttributeSet::GetCurrentHealthAttribute());
	float MaxHealth = ASC->GetNumericAttribute(UVFAttributeSet::GetMaxHealthAttribute());
	if (HealthBar) HealthBar->UpdateValues(CurrentHealth, MaxHealth);

	float CurrentStamina = ASC->GetNumericAttribute(UVFAttributeSet::GetCurrentStaminaAttribute());
	float MaxStamina = ASC->GetNumericAttribute(UVFAttributeSet::GetMaxStaminaAttribute());
	if (StaminaBar) StaminaBar->UpdateValues(CurrentStamina, MaxStamina);

	float CurrentPosture = ASC->GetNumericAttribute(UVFAttributeSet::GetCurrentPostureAttribute());
	float MaxPosture = ASC->GetNumericAttribute(UVFAttributeSet::GetMaxPostureAttribute());
	if (PostureBar) PostureBar->UpdateValues(CurrentPosture, MaxPosture);
}

 void UVFHUDWidget::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (HealthBar && OwnerASC.IsValid())
	{
		float CurrentHealth = OwnerASC->GetNumericAttribute(UVFAttributeSet::GetCurrentHealthAttribute());
		float MaxHealth = OwnerASC->GetNumericAttribute(UVFAttributeSet::GetMaxHealthAttribute());
		HealthBar->UpdateValues(CurrentHealth, MaxHealth);
	}
}

void UVFHUDWidget::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	if (StaminaBar && OwnerASC.IsValid())
	{
		float CurrentStamina = OwnerASC->GetNumericAttribute(UVFAttributeSet::GetCurrentStaminaAttribute());
		float MaxStamina = OwnerASC->GetNumericAttribute(UVFAttributeSet::GetMaxStaminaAttribute());
		StaminaBar->UpdateValues(CurrentStamina, MaxStamina);
	}
}

void UVFHUDWidget::OnPostureChanged(const FOnAttributeChangeData& Data)
{
	if (PostureBar && OwnerASC.IsValid())
	{
		float CurrentPosture = OwnerASC->GetNumericAttribute(UVFAttributeSet::GetCurrentPostureAttribute());
		float MaxPosture = OwnerASC->GetNumericAttribute(UVFAttributeSet::GetMaxPostureAttribute());
		PostureBar->UpdateValues(CurrentPosture, MaxPosture);
	}
}
