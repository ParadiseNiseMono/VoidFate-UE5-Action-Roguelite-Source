// Paradise NiseMono All Rights Reserved


#include "Characters/VFNinjaCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "VFDebugHelper.h"
#include "VFFunctionLibrary.h"
#include "VFGameplayTags.h"
#include "AbilitySystem/VFAbilitySystemComponent.h"
#include "AbilitySystem/VFAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Combat/NinjaCombatComponent.h"
#include "Components/Input/VFInputComponent.h"
#include "Components/UI/NinjaUIComponent.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameModes/VFBaseGameMode.h"
#include "VoidFateSettings/VFGameUserSettings.h"
#include "Characters/VFEnemyCharacter.h"

AVFNinjaCharacter::AVFNinjaCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 65.0f);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bInheritPitch = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 450.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxAcceleration = 1500.f;

	NinjaCombatComponent = CreateDefaultSubobject<UNinjaCombatComponent>(TEXT("NinjaCombatComponent"));

	NinjaUIComponent = CreateDefaultSubobject<UNinjaUIComponent>(TEXT("NinjaUIComponent"));
}

UPawnCombatComponent* AVFNinjaCharacter::GetPawnCombatComponent() const
{
	return NinjaCombatComponent;
}

UPawnUIComponent* AVFNinjaCharacter::GetPawnUIComponent() const
{
	return NinjaUIComponent;
}

UNinjaUIComponent* AVFNinjaCharacter::GetNinjaUIComponent() const
{
	return NinjaUIComponent;
}

void AVFNinjaCharacter::OnLoadingScreenActivated_Implementation()
{
	DisableInput(GetController<APlayerController>());
}

void AVFNinjaCharacter::OnLoadingScreenDeactivated_Implementation()
{
	EnableInput(GetController<APlayerController>());
}

void AVFNinjaCharacter::ResolveDashPenetrationAndRestoreCollision()
{
	TArray<AActor*> OverlappingActors;
	GetCapsuleComponent()->GetOverlappingActors(OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor && Cast<AVFEnemyCharacter>(OverlappingActor))
		{
			FVector PlayerLoc = GetActorLocation();
			FVector EnemyLoc = OverlappingActor->GetActorLocation();

			FVector PushDir = (PlayerLoc - EnemyLoc).GetSafeNormal();

			PushDir.Z = 0.f;
			PushDir.Normalize();

			float EnemyRadius = Cast<ACharacter>(OverlappingActor)->GetCapsuleComponent()->GetScaledCapsuleRadius();
			float PlayerRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();

			float SafeDistance = EnemyRadius + PlayerRadius + 15.f;

			FVector NewLoc = EnemyLoc + (PushDir * SafeDistance);
			NewLoc.Z = PlayerLoc.Z;

			SetActorLocation(NewLoc, false);
		}
	}

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}

void AVFNinjaCharacter::UpdateVerticalCameraLock(bool bLock)
{
	CameraBoom->bInheritPitch = !bLock;

	if (bLock)
	{
		FRotator CurrentRot = CameraBoom->GetRelativeRotation();

		CurrentRot.Pitch = -15.0f;

		CameraBoom->SetRelativeRotation(CurrentRot);
	}
}

void AVFNinjaCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	UVFGameUserSettings* VFGameUserSettings = UVFGameUserSettings::Get();

	CameraBoom->TargetArmLength = FMath::Lerp(CameraMinDistance, CameraMaxDistance, VFGameUserSettings->GetCameraZoomDistance());
	UpdateVerticalCameraLock(VFGameUserSettings->GetLockVerticalCamera());

	VFGameUserSettings->OnCameraZoomDistanceChanged.BindUObject(this, &ThisClass::SetCameraZoomDistance);
	VFGameUserSettings->OnLockVerticalCameraChanged.BindUObject(this, &ThisClass::UpdateVerticalCameraLock);
}

void AVFNinjaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDataAsset, TEXT("InputConfigDataAsset is not set"));
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);
	Subsystem->AddMappingContext(InputConfigDataAsset->InputMappingContext, 0);

	UVFInputComponent* VFInputComponent = CastChecked<UVFInputComponent>(InputComponent);

	VFInputComponent->BindNativeInputAction(InputConfigDataAsset, VFGameplayTags::Input_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	VFInputComponent->BindNativeInputAction(InputConfigDataAsset, VFGameplayTags::Input_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	VFInputComponent->BindNativeInputAction(InputConfigDataAsset, VFGameplayTags::Input_PickUp, ETriggerEvent::Started, this, &ThisClass::Input_PickUpItemsStarted);

	VFInputComponent->BindNativeInputAction(InputConfigDataAsset, VFGameplayTags::Input_SwitchTarget, ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTargetTriggered);
	VFInputComponent->BindNativeInputAction(InputConfigDataAsset, VFGameplayTags::Input_SwitchTarget, ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetCompleted);

	VFInputComponent->BindNativeInputAction(InputConfigDataAsset, VFGameplayTags::Input_Sprint, ETriggerEvent::Started, this, &ThisClass::Input_SprintStarted);
	VFInputComponent->BindNativeInputAction(InputConfigDataAsset, VFGameplayTags::Input_Sprint, ETriggerEvent::Completed, this, &ThisClass::Input_SprintCompleted);
	VFInputComponent->BindNativeInputAction(InputConfigDataAsset, VFGameplayTags::Input_Sprint, ETriggerEvent::Canceled, this, &ThisClass::Input_SprintCanceled);
	
	VFInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputHeld, &ThisClass::Input_AbilityInputReleased);
}

void AVFNinjaCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(VFAbilitySystemComponent);

			if (AVFBaseGameMode* BaseGameMode = GetWorld()->GetAuthGameMode<AVFBaseGameMode>())
			{
				switch (BaseGameMode->GetCurrentGameDifficulty())
				{
				case EVFGameDifficulty::Easy:
					/*Debug::Print(TEXT("Current Difficulty: Easy"));*/
					break;

				case EVFGameDifficulty::Normal:
					/*Debug::Print(TEXT("Current Difficulty: Normal"));*/
					break;

				case EVFGameDifficulty::Hard:
					/*Debug::Print(TEXT("Current Difficulty: Hard"));*/
					break;

				case EVFGameDifficulty::VeryHard:
					/*Debug::Print(TEXT("Current Difficulty: Very Hard"));*/
					break;

				default:
					break;
				}
			}
		}
	}

	if (VFAbilitySystemComponent)
	{
		VFAbilitySystemComponent->RegisterGameplayTagEvent(
			VFGameplayTags::Player_Status_Transcendence_Active
		).AddUObject(this, &ThisClass::OnTranscendenceActivated);

	}
	
}

void AVFNinjaCharacter::OnStunTagChanged(const FGameplayTag StunTag, int32 NewCount)
{
	if (NewCount == 0 && !UVFFunctionLibrary::NativeDoesActorHaveTag(this, VFGameplayTags::Shared_Status_Dead) && VFAttributeSet && VFAbilitySystemComponent)
	{
		UVFAttributeSet* AttributeSet = VFAttributeSet;
		float HalfPosture = AttributeSet->GetMaxPosture() * 0.5f;

		VFAbilitySystemComponent->ApplyModToAttributeUnsafe(UVFAttributeSet::GetCurrentPostureAttribute(), EGameplayModOp::Override, HalfPosture);
		UVFFunctionLibrary::RemoveGameplayTagFromActorIfFound(this, VFGameplayTags::Shared_Status_Posture_Full);
	}
}

void AVFNinjaCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AVFNinjaCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}
	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);	
	}
}

void AVFNinjaCharacter::Input_Jump(const FInputActionValue& InputActionValue)
{
	//This game can't jump
}

void AVFNinjaCharacter::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	SwitchDirection = InputActionValue.Get<FVector2D>();
}

void AVFNinjaCharacter::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		SwitchDirection.X>0.f? VFGameplayTags::Player_Event_SwitchTarget_Right : VFGameplayTags::Player_Event_SwitchTarget_Left,
		Data
	);
}

void AVFNinjaCharacter::Input_PickUpItemsStarted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		VFGameplayTags::Player_Event_PickUpItems,
		Data
	);
}

void AVFNinjaCharacter::Input_SprintStarted(const FInputActionValue& InputActionValue)
{
	UVFFunctionLibrary::AddGameplayTagToActorIfNone(this, VFGameplayTags::Player_Status_Movement_Sprint);
}

void AVFNinjaCharacter::Input_SprintCompleted(const FInputActionValue& InputActionValue)
{
	UVFFunctionLibrary::RemoveGameplayTagFromActorIfFound(this, VFGameplayTags::Player_Status_Movement_Sprint);
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
}

void AVFNinjaCharacter::Input_SprintCanceled(const FInputActionValue& InputActionValue)
{
	UVFFunctionLibrary::RemoveGameplayTagFromActorIfFound(this, VFGameplayTags::Player_Status_Movement_Sprint);
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
}

void AVFNinjaCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	VFAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AVFNinjaCharacter::Input_AbilityInputHeld(FGameplayTag InInputTag)
{
	VFAbilitySystemComponent->OnAbilityInputHeld(InInputTag);
}

void AVFNinjaCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	VFAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}

void AVFNinjaCharacter::SetCameraZoomDistance(float InZoomDistance)
{
	CameraBoom->TargetArmLength = FMath::Lerp(CameraMinDistance, CameraMaxDistance, InZoomDistance);
}

void AVFNinjaCharacter::OnTranscendenceActivated(const FGameplayTag StatusTag, int32 NewCount)
{
	GetNinjaUIComponent()->OnStatusTagGranted.Broadcast(StatusTag, NewCount);
}
