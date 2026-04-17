// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/VFUserWidgetBase.h"
#include "GameModes/VFTutorialGameMode.h"
#include "CommonInputTypeEnum.h"
#include "VFTutorialModalWidget.generated.h"

/**
 * 
 */
UCLASS()
class VOIDFATE_API UVFTutorialModalWidget : public UVFUserWidgetBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandlePhaseChanged(EVFTutorialPhase OldPhase, EVFTutorialPhase NewPhase);

	UFUNCTION(BlueprintImplementableEvent, Category = "Tutorial|UI")
	void OnUpdateTutorialUI(EVFTutorialPhase NewPhase);

	UFUNCTION()
	void HandleInputMethodChanged(ECommonInputType CurrentInputType);

	UFUNCTION(BlueprintImplementableEvent, Category = "Tutorial|UI")
	void OnInputDeviceUpdated(ECommonInputType NewInputType);
};
