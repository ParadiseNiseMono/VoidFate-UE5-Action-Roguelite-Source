// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_PlayerState_Stats.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class VOIDFATE_API UWidget_PlayerState_Stats : public UWidget_ActivatableBase
{
	GENERATED_BODY()
	
protected:
	//~Begin CommonActivatableWidget Interface
	virtual void NativeOnActivated() override;
	//~End CommonActivatableWidget Interface

	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_MaxHealth;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_CurrentHealth;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_MaxStamina;
	
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_CurrentStamina;
	
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_MaxPosture;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_CurrentPosture;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_PostureDamage;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_AttackPower;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Defense;
	//***** Bound Widgets *****//
};
