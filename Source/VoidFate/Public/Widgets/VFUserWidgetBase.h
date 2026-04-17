// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widget_ActivatableBase.h"
#include "Blueprint/UserWidget.h"
#include "VFUserWidgetBase.generated.h"

class UEnemyUIComponent;
class UNinjaUIComponent;
/**
 * 
 */
UCLASS()
class VOIDFATE_API UVFUserWidgetBase : public UWidget_ActivatableBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "On Owning Ninja UI Component Initialized"))
	void BP_OnOwningNinjaUIComponentInitialized(UNinjaUIComponent* OwningNinjaUIComponent);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "On Owning Enemy UI Component Initialized"))
	void BP_OnOwningEnemyUIComponentInitialized(UEnemyUIComponent* OwningEnemyUIComponent);
public:
	UFUNCTION(BlueprintCallable)
	void InitEnemyCreatedWidget(AActor* OwningEnemyActor);
};
