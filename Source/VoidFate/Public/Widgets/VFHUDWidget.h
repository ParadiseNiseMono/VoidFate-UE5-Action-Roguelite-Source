// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "VFUserWidgetBase.h"
#include "VFHUDWidget.generated.h"

class UNinjaUIComponent;
class UVFStatBarWidget;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class VOIDFATE_API UVFHUDWidget : public UVFUserWidgetBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void InitializeHUD(UAbilitySystemComponent* ASC);

protected:
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UVFStatBarWidget> HealthBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UVFStatBarWidget> StaminaBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UVFStatBarWidget> PostureBar;

private:
	TWeakObjectPtr<UAbilitySystemComponent> OwnerASC;
	
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnStaminaChanged(const FOnAttributeChangeData& Data);
	void OnPostureChanged(const FOnAttributeChangeData& Data);
};
