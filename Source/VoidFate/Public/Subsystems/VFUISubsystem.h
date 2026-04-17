// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VFTypes/VFEnumTypes.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "VFUISubsystem.generated.h"

class UVFCommonButtonBase;
class UWidget_ActivatableBase;
struct FGameplayTag;
class UWidget_PrimaryLayout;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionTextUpdatedDelegage, UVFCommonButtonBase*, BroadcastingButton, FText, DescriptionText);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeybindingChangedDelegate);

/**
 * 
 */
UCLASS()
class VOIDFATE_API UVFUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UVFUISubsystem* Get(const UObject* WorldContextObject);

	// ~Begin USubsystem Interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	// ~End USubsystem Interface

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget);

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedHUDWidget(UWidget_ActivatableBase* InCreatedWidget);

	void PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag,
		TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushStateCallback);
	void PushConfirmScreenToModalStackAsync(EConfirmScreenType InScreenType,
		const FText& InScreenTitle, const FText& InScreenMsg, TFunction<void(EConfirmScreenButtonType)> ButtonClickedCallback);

	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionTextUpdatedDelegage OnButtonDescriptionTextUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnKeybindingChangedDelegate OnKeybindingChanged;
private:
	UPROPERTY(Transient)
	UWidget_PrimaryLayout* CreatedPrimaryLayout;

	UPROPERTY(Transient)
	UWidget_ActivatableBase* CreatedHUDWidget;

public:
	UFUNCTION(BlueprintPure)
	UWidget_PrimaryLayout* GetCreatedPrimaryLayout() const {return CreatedPrimaryLayout;}

	UFUNCTION(BlueprintPure)
	UWidget_ActivatableBase* GetCreatedHUDWidget() const { return CreatedHUDWidget; }
};
