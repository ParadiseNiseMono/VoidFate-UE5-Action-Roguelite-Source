// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/VFBaseCharacter.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Interfaces/VFLoadingScreenInterface.h"
#include "VFNinjaCharacter.generated.h"

class UNinjaUIComponent;
class UNinjaCombatComponent;
DECLARE_DELEGATE(FOnJumpActionTriggered);

struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class VOIDFATE_API AVFNinjaCharacter : public AVFBaseCharacter, public IVFLoadingScreenInterface
{
	GENERATED_BODY()

public:
	AVFNinjaCharacter();

	FOnJumpActionTriggered OnJumpActionTriggered;

	//~ Begin IPawnCombatInterface	Interface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface

	//~ Begin IPawnUIInterface	Interface
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UNinjaUIComponent* GetNinjaUIComponent() const override;
	//~ End IPawnUIInterface Interface

	//~ Begin IVFLoadingScreenInterface	Interface
	virtual void OnLoadingScreenActivated_Implementation() override;
	virtual void OnLoadingScreenDeactivated_Implementation() override;
	//~ End IVFLoadingScreenInterface Interface

	UFUNCTION(BlueprintCallable, Category = "VoidFate|Combat")
	void ResolveDashPenetrationAndRestoreCollision();

	UFUNCTION(BlueprintCallable, Category = "Void|Camera")
	void UpdateVerticalCameraLock(bool bLock);

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	//~ Begin APawn	Interface
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	//~ Begin AVFBaseCharacter Interface
	virtual void OnStunTagChanged(const FGameplayTag StunTag, int32 NewCount) override;
	//~ End AVFBaseCharacter Interface
private:
	
#pragma region Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float CameraMaxDistance = 1000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float CameraMinDistance = 200.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UNinjaCombatComponent* NinjaCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UNinjaUIComponent* NinjaUIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float DefaultMaxWalkSpeed = 0.f;
	
#pragma endregion

#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Data", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	UPROPERTY()
	FVector2D SwitchDirection = FVector2D::ZeroVector;

	void Input_Move(const FInputActionValue& InputActionValue);

	void Input_Look(const FInputActionValue& InputActionValue);

	void Input_Jump(const FInputActionValue& InputActionValue);

	void Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue);
	void Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue);

	void Input_PickUpItemsStarted(const FInputActionValue& InputActionValue);

	void Input_SprintStarted(const FInputActionValue& InputActionValue);
	void Input_SprintCompleted(const FInputActionValue& InputActionValue);
	void Input_SprintCanceled(const FInputActionValue& InputActionValue);

	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputHeld(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);

#pragma endregion

	void SetCameraZoomDistance(float InZoomDistance);

	void OnTranscendenceActivated(const FGameplayTag StatusTag, int32 NewCount);

public:
	FORCEINLINE UNinjaCombatComponent* GetNinjaCombatComponent() const { return NinjaCombatComponent; }
	FORCEINLINE float GetDefaultMaxWalkSpeed() const { return DefaultMaxWalkSpeed;  }
};
