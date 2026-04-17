// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/VFBaseAnimInstance.h"
#include "VFCharacterAnimInstance.generated.h"

class AVFNinjaCharacter;

UENUM(BlueprintType)
enum class EVFJumpDirection : uint8
{
	Stay,
	Forward,
	Backward,
	Left,
	Right
};

/**
 * 
 */
UCLASS()
class VOIDFATE_API UVFCharacterAnimInstance : public UVFBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	void OnJump();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|References")
	AVFNinjaCharacter* OwningNinjaCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimData|LocomotionData")
	bool bJumped = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	EVFJumpDirection CurrentJumpDirection = EVFJumpDirection::Stay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float EnterRelaxStateThreshold = 5.f;

	float IdleElpasedTime;
};


