// Paradise NiseMono All Rights Reserved


#include "Subsystems/VFExecutionCameraSubsystem.h"

#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "CineCameraComponent.h"

void UVFExecutionCameraSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CinematicCameraActor = InWorld.SpawnActor<ACameraActor>(ACameraActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (CinematicCameraActor)
	{
		CinematicCameraActor->SetActorHiddenInGame(true);
	}
}

void UVFExecutionCameraSubsystem::Deinitialize()
{
	if (CinematicCameraActor && CinematicCameraActor->IsValidLowLevel())
	{
		CinematicCameraActor->Destroy();
	}
	Super::Deinitialize();
}

FVector UVFExecutionCameraSubsystem::CalculateBestCameraLocation(AActor* Attacker, AActor* Victim, FVector IdealOffset)
{
	FVector AttackerLoc = Attacker->GetActorLocation();
	FVector VictimLoc = Victim->GetActorLocation();

	FVector Direction = (VictimLoc - AttackerLoc).GetSafeNormal();
	FVector UpVector = FVector::UpVector;
	FVector RightVector = FVector::CrossProduct(Direction, UpVector);

	FVector CenterPoint = (AttackerLoc + VictimLoc) * 0.5f;

	FVector IdealLocation = CenterPoint
		+ (-Direction * IdealOffset.X) 
		+ (RightVector * IdealOffset.Y)
		+ (UpVector * IdealOffset.Z);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Attacker);
	QueryParams.AddIgnoredActor(Victim);
	QueryParams.AddIgnoredActor(CinematicCameraActor);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		CenterPoint, // 起點：兩人中間
		IdealLocation, // 終點：理想攝影機位置
		ECC_Camera,
		QueryParams
	);

	if (bHit)
	{
		// 如果撞到牆壁，就把攝影機放在牆壁前面一點點
		return HitResult.Location + (CenterPoint - IdealLocation).GetSafeNormal() * 10.0f;
	}

	return IdealLocation;
}

void UVFExecutionCameraSubsystem::StartCinematicExecution(AActor* Attacker, AActor* Victim, FVector CameraOffset, float BlendDuration)
{
	if (!Attacker || !Victim || !CinematicCameraActor) return;

	TargetPC = UGameplayStatics::GetPlayerController(this, 0);
	if (!TargetPC) return;

	FVector BestLocation = CalculateBestCameraLocation(Attacker, Victim, CameraOffset);

	FVector MidPoint = (Attacker->GetActorLocation() + Victim->GetActorLocation()) * 0.5f;
	MidPoint.Z += 50.0f;
	FRotator LookAtRotation = (MidPoint - BestLocation).Rotation();

	CinematicCameraActor->SetActorLocation(BestLocation);
	CinematicCameraActor->SetActorRotation(LookAtRotation);

	TargetPC->SetViewTargetWithBlend(CinematicCameraActor, BlendDuration, VTBlend_Cubic, 0.5f);
}

void UVFExecutionCameraSubsystem::StopCinematicExecution(float BlendDuration)
{
	if (TargetPC)
	{
		TargetPC->SetViewTargetWithBlend(TargetPC->GetPawn(), BlendDuration, VTBlend_Cubic, 0.5f);
	}
}

void UVFExecutionCameraSubsystem::StartBossIntro(AActor* BossActor, FText BossAbstractName, float IntroDuration)
{
	UWorld* World = GetWorld();
	if (!World || !BossActor) return;

	const float RealTimeDelay = IntroDuration;
	const float BossIntroDilation = 0.3f;

	UGameplayStatics::SetGlobalTimeDilation(World, BossIntroDilation);

	const float ScaledDelay = RealTimeDelay * BossIntroDilation;

	TargetPC = UGameplayStatics::GetPlayerController(World, 0);
	if (TargetPC)
	{
		UCineCameraComponent* BossCamera = BossActor->FindComponentByClass<UCineCameraComponent>();

		TargetPC->SetCinematicMode(true, false, false, true, true);

		if (BossCamera)
		{
			BossCamera->Activate();
			TargetPC->SetViewTargetWithBlend(BossActor, 0.1f, EViewTargetBlendFunction::VTBlend_Cubic);
		}
		else
		{
			TargetPC->SetViewTargetWithBlend(BossActor, 0.1f, EViewTargetBlendFunction::VTBlend_Cubic);
		}

		APawn* PlayerPawn = TargetPC->GetPawn();
		if (PlayerPawn)
		{
			PlayerPawn->DisableInput(TargetPC);
		}
	}

	OnBossIntroTriggered.Broadcast(BossAbstractName);

	World->GetTimerManager().SetTimer(
		BossIntroTimerHandle,
		this,
		&UVFExecutionCameraSubsystem::StopBossIntro,
		ScaledDelay,
		false
	);
}

void UVFExecutionCameraSubsystem::StopBossIntro()
{
	UWorld* World = GetWorld();
	if (!World) return;

	UGameplayStatics::SetGlobalTimeDilation(World, 1.0f);

	if (TargetPC)
	{
		TargetPC->SetCinematicMode(false, false, false, true, true);

		TargetPC->SetViewTargetWithBlend(TargetPC->GetPawn(), 0.1f, EViewTargetBlendFunction::VTBlend_Cubic);

		APawn* PlayerPawn = TargetPC->GetPawn();
		if (PlayerPawn)
		{
			PlayerPawn->EnableInput(TargetPC);
		}
	}
}
