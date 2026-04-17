// Paradise NiseMono All Rights Reserved


#include "AbilitySystem/AbilityTasks/AbilityTask_PlayCinematicCamera.h"
#include <Subsystems/VFExecutionCameraSubsystem.h>

UAbilityTask_PlayCinematicCamera* UAbilityTask_PlayCinematicCamera::PlayCinematicCamera(UGameplayAbility* OwningAbility, AActor* Attacker, AActor* Victim, FVector CameraOffset, float BlendDuration)
{
	UAbilityTask_PlayCinematicCamera* MyTask = NewAbilityTask<UAbilityTask_PlayCinematicCamera>(OwningAbility);
	MyTask->CachedAttacker = Attacker;
	MyTask->CachedVictim = Victim;
	MyTask->CachedOffset = CameraOffset;
	MyTask->CachedBlendDuration = BlendDuration;
	return MyTask;
}

void UAbilityTask_PlayCinematicCamera::Activate()
{
	Super::Activate();

	UWorld* World = GetWorld();
	if (!World) return;

	if (UVFExecutionCameraSubsystem* CamSubsystem = World->GetSubsystem<UVFExecutionCameraSubsystem>())
	{
		CamSubsystem->StartCinematicExecution(CachedAttacker, CachedVictim, CachedOffset, CachedBlendDuration);
	}
}

void UAbilityTask_PlayCinematicCamera::OnDestroy(bool bInOwnerFinished)
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (UVFExecutionCameraSubsystem* CamSubsystem = World->GetSubsystem<UVFExecutionCameraSubsystem>())
		{
			CamSubsystem->StopCinematicExecution(0.4f);
		}
	}

	Super::OnDestroy(bInOwnerFinished);
}
