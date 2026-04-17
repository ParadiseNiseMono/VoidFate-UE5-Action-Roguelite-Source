// Paradise NiseMono All Rights Reserved

#include "Subsystems/VFCombatFeedbackSubsystem.h"

#include "Camera/VFCameraModifier_FOVImpact.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Curves/CurveFloat.h"
#include "Camera/VFCameraModifier_PostProcess.h"

void UVFCombatFeedbackSubsystem::ApplyHitStop(AActor* TargetActor, float Duration, float TimeDilation)
{
	if (!IsValid(TargetActor)) return;

	UWorld* World = GetWorld();
	if (!World) return;

	TWeakObjectPtr<AActor> WeakTarget(TargetActor);

	TargetActor->CustomTimeDilation = TimeDilation;

	FTimerManager& TimerManager = World->GetTimerManager();

	if (FTimerHandle* ExistingTimer = ActiveHitStopTimers.Find(WeakTarget))
	{
		TimerManager.ClearTimer(*ExistingTimer);
	}

	FTimerHandle NewTimerHandle;
	TimerManager.SetTimer(
		NewTimerHandle,
		FTimerDelegate::CreateUObject(this, &UVFCombatFeedbackSubsystem::RestoreTimeDilation, WeakTarget),
		Duration,
		false
	);

	ActiveHitStopTimers.Add(WeakTarget, NewTimerHandle);
}

void UVFCombatFeedbackSubsystem::ApplyHitStopToBoth(AActor* Attacker, AActor* Defender, float Duration, float TimeDilation)
{
	ApplyHitStop(Attacker, Duration, TimeDilation);
	ApplyHitStop(Defender, Duration, TimeDilation);
}

void UVFCombatFeedbackSubsystem::PlayFOVImpact(APlayerController* PC, TSubclassOf<UVFCameraModifier_FOVImpact> ModifierClass, float FOVOffset, float Duration, UCurveFloat* ImpactCurve)
{
	if (!PC || !PC->PlayerCameraManager || !ModifierClass || !ImpactCurve) return;

	UCameraModifier* NewModifier = PC->PlayerCameraManager->AddNewCameraModifier(ModifierClass);

	if (UVFCameraModifier_FOVImpact* FOVModifier = Cast<UVFCameraModifier_FOVImpact>(NewModifier))
	{
		FOVModifier->InitFOVImpact(FOVOffset, Duration, ImpactCurve);
	}
}

void UVFCombatFeedbackSubsystem::PlayPostProcessFlash(TSubclassOf<class UVFCameraModifier_PostProcess> ModifierClass, UMaterialInterface* Material, UCurveFloat* StrengthCurve)
{
	if (!ModifierClass || !Material || !StrengthCurve) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC || !PC->PlayerCameraManager) return;

	UVFCameraModifier_PostProcess* PPModifier = Cast<UVFCameraModifier_PostProcess>(
		PC->PlayerCameraManager->AddNewCameraModifier(ModifierClass)
	);

	if (PPModifier)
	{
		PPModifier->InitPostProcessFlash(Material, StrengthCurve);
	}
}

void UVFCombatFeedbackSubsystem::StopAllHitStops()
{
	UWorld* World = GetWorld();
	if (!World) return;

	FTimerManager& TimerManager = World->GetTimerManager();

	for (auto& Pair : ActiveHitStopTimers)
	{
		if (AActor* ValidActor = Pair.Key.Get())
		{
			ValidActor->CustomTimeDilation = 1.0f;
		}

		TimerManager.ClearTimer(Pair.Value);
	}

	ActiveHitStopTimers.Empty();
}


void UVFCombatFeedbackSubsystem::PlayDirectionalCameraShake(APlayerController* PC, TSubclassOf<class UCameraShakeBase> ShakeClass, FVector HitDirection, float Scale)
{
	if (!PC || !PC->PlayerCameraManager || !ShakeClass) return;

	FVector NormalizedDirection = HitDirection.GetSafeNormal();

	FRotator ShakeRotation = NormalizedDirection.Rotation();

	PC->PlayerCameraManager->StartCameraShake(
		ShakeClass,
		Scale,
		ECameraShakePlaySpace::UserDefined,
		ShakeRotation
	);
}

UVFCameraModifier_PostProcess* UVFCombatFeedbackSubsystem::AddPersistentPostProcess(TSubclassOf<class UVFCameraModifier_PostProcess> ModifierClass, UMaterialInterface* Material)
{
	if (!ModifierClass || !Material) return nullptr;

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC || !PC->PlayerCameraManager) return nullptr;

	UVFCameraModifier_PostProcess* PPModifier = Cast<UVFCameraModifier_PostProcess>(
		PC->PlayerCameraManager->AddNewCameraModifier(ModifierClass)
	);

	if (PPModifier)
	{
		PPModifier->InitPersistentPostProcess(Material);
	}

	return PPModifier;
}

void UVFCombatFeedbackSubsystem::RemovePersistentPostProcess(UVFCameraModifier_PostProcess* Modifier)
{
	if (Modifier)
	{
		Modifier->DisableModifier(true);
	}
}

void UVFCombatFeedbackSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void UVFCombatFeedbackSubsystem::Deinitialize()
{
	ActiveHitStopTimers.Empty();
	Super::Deinitialize();
}

void UVFCombatFeedbackSubsystem::RestoreTimeDilation(TWeakObjectPtr<AActor> WeakActor)
{
	if (AActor* ValidActor = WeakActor.Get())
	{
		ValidActor->CustomTimeDilation = 1.0f;
	}

	ActiveHitStopTimers.Remove(WeakActor);
}
