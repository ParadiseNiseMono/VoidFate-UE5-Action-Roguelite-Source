// Paradise NiseMono All Rights Reserved


#include "Components/VFGhostTrailComponent.h"

#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include <Items/GhostTrail/VFGhostActor.h>

UVFGhostTrailComponent::UVFGhostTrailComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UVFGhostTrailComponent::StartGhostTrail()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(SpawnTimerHandle)) return;

	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (OwnerChar)
	{
		OwnerMesh = OwnerChar->GetMesh();
		if (OwnerMesh && GhostMaterial)
		{
			float SpawnInterval = 1.0f / SpawnRate;
			GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UVFGhostTrailComponent::SpawnGhost, SpawnInterval, true, 0.0f);
		}
	}
}

void UVFGhostTrailComponent::StopGhostTrail()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
}


void UVFGhostTrailComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UVFGhostTrailComponent::SpawnGhost()
{
	if (!OwnerMesh || !GetWorld()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AVFGhostActor* SpawnedGhost = GetWorld()->SpawnActor<AVFGhostActor>(
		AVFGhostActor::StaticClass(),
		OwnerMesh->GetComponentLocation(),
		OwnerMesh->GetComponentRotation(),
		SpawnParams
	);

	if (SpawnedGhost)
	{
		SpawnedGhost->InitGhost(OwnerMesh, GhostMaterial, GhostLifetime);
	}
}

void UVFGhostTrailComponent::DestroyGhost(UPoseableMeshComponent* MeshToDestroy)
{
	if (MeshToDestroy)
	{
		MeshToDestroy->DestroyComponent();
	}
}

