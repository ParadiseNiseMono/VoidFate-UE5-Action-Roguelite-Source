// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "VFBaseAnimInstance.generated.h"

class UCharacterMovementComponent;
class AVFBaseCharacter;
/**
 * 
 */
UCLASS()
class VOIDFATE_API UVFBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	AVFBaseCharacter* OwingCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwingMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bFalling;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LocomotionDirection;

	UPROPERTY(BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LeanAngle;

	UPROPERTY(BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float BrakePitch;

	FRotator LastFrameRotation;

	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	bool DoesOwnerHasTag(FGameplayTag TagToCheck) const;

private:
	void CalculateLeanAngle(float DeltaSeconds);

	void CalculateBrakePitch(float DeltaSeconds);
};
