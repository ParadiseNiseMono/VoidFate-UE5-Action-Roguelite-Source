// Paradise NiseMono All Rights Reserved


#include "AnimInstances/VFBaseAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "VFFunctionLibrary.h"
#include "Characters/VFBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>

void UVFBaseAnimInstance::NativeInitializeAnimation()
{
	OwingCharacter = Cast<AVFBaseCharacter>(TryGetPawnOwner());

	if (OwingCharacter)
	{
		OwingMovementComponent = OwingCharacter->GetCharacterMovement();
	}
}

void UVFBaseAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwingCharacter || !OwingMovementComponent) return;

	GroundSpeed = OwingCharacter->GetVelocity().Size2D();

	bHasAcceleration = OwingMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;
	bFalling = OwingMovementComponent->IsFalling();
	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwingCharacter->GetVelocity(), OwingCharacter->GetActorRotation());

	CalculateLeanAngle(DeltaSeconds);
	CalculateBrakePitch(DeltaSeconds);
}

void UVFBaseAnimInstance::CalculateLeanAngle(float DeltaSeconds)
{
	FRotator CurrentRotation = OwingCharacter->GetActorRotation();
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(CurrentRotation, LastFrameRotation);

	float YawSpeed = DeltaRot.Yaw / DeltaSeconds;

	float TargetLean = FMath::GetMappedRangeValueClamped(
		FVector2D(-500.f, 500.f), // 輸入範圍 (轉速)
		FVector2D(-25.f, 25.f),   // 輸出範圍 (傾斜度)
		YawSpeed
	);

	LeanAngle = FMath::FInterpTo(LeanAngle, TargetLean, DeltaSeconds, 6.0f);

	LastFrameRotation = CurrentRotation;
}

void UVFBaseAnimInstance::CalculateBrakePitch(float DeltaSeconds)
{
	float TargetBrakePitch = 0.0f;

	if (!bHasAcceleration && GroundSpeed > 20.0f)
	{
		TargetBrakePitch = FMath::GetMappedRangeValueClamped(
			FVector2D(0.f, 600.f),
			FVector2D(0.f, -15.f),
			GroundSpeed
		);
	}

	float InterpSpeed = bHasAcceleration ? 10.0f : 15.0f;
	BrakePitch = FMath::FInterpTo(BrakePitch, TargetBrakePitch, DeltaSeconds, InterpSpeed);
}

bool UVFBaseAnimInstance::DoesOwnerHasTag(FGameplayTag TagToCheck) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UVFFunctionLibrary::NativeDoesActorHaveTag(OwningPawn, TagToCheck);
	}
	return false;
}
