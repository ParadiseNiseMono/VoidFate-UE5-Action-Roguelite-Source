// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Widget_ActivatableBase.generated.h"

class AVFNinjaController;
class AVFUIController;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class VOIDFATE_API UWidget_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure)
	AVFUIController* GetOwningVFPlayerController();

	UFUNCTION(BlueprintPure)
	AVFNinjaController* GetOwningVFNanjaController();
private:
	TWeakObjectPtr<AVFUIController> CachedOwningVFPlayerController;
	TWeakObjectPtr<AVFNinjaController> CachedOwningVFNinjaController;
};
