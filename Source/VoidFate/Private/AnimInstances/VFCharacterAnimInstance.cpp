// Paradise NiseMono All Rights Reserved


#include "AnimInstances/VFCharacterAnimInstance.h"

#include "Characters/VFNinjaCharacter.h"

void UVFCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningNinjaCharacter = Cast<AVFNinjaCharacter>(OwingCharacter);

	if (OwningNinjaCharacter && !OwningNinjaCharacter->OnJumpActionTriggered.IsBoundToObject(this))
	{
		OwningNinjaCharacter->OnJumpActionTriggered.BindUObject(this, &ThisClass::OnJump);
	}
}

void UVFCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (bHasAcceleration)
	{
		IdleElpasedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	else
	{
		IdleElpasedTime += DeltaSeconds;
		bShouldEnterRelaxState = (IdleElpasedTime >= EnterRelaxStateThreshold);
	}
}

void UVFCharacterAnimInstance::OnJump()
{
	//TODO: 等處理完跳躍能力再來搞這邊的邏輯
}
