// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "VFUserWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "VFStatBarWidget.generated.h"

class UNinjaUIComponent;
class UProgressBar;
/**
 * 
 */
UCLASS()
class VOIDFATE_API UVFStatBarWidget : public UVFUserWidgetBase
{
	GENERATED_BODY()

public:
	UVFStatBarWidget();
	//~ Begin UUserWidget Interface
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~ End UUserWidget Interface
	
	UFUNCTION(BlueprintCallable)
	void UpdateValues(float CurrentValue, float MaxValue);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "On Stat Bar Percent Changed"))
	void BP_OnStatBarPercentChanged(float InPercent);

protected:
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> Bar_Current;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> Bar_Ghost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float GhostInterpSpeed = 2.0f;
	
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "On State Bar Increased"))
	void BP_OnStateBarIncreased();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "On State Bar Decreased"))
	void BP_OnStateBarDecreased();

private:
	float TargetPercent = 1.0f;
	float GhostPercent = 1.0f;
};
