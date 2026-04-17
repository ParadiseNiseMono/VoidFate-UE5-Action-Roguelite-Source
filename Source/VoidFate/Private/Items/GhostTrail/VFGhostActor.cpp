// Paradise NiseMono All Rights Reserved


#include "Items/GhostTrail/VFGhostActor.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

AVFGhostActor::AVFGhostActor()
{
	PrimaryActorTick.bCanEverTick = false;

	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMesh"));
	RootComponent = PoseableMesh;
}

void AVFGhostActor::InitGhost(USkeletalMeshComponent* PoseSource, UMaterialInterface* GhostMaterial, float Lifetime)
{
	if (!PoseSource) return;

	PoseableMesh->SetSkinnedAssetAndUpdate(PoseSource->GetSkeletalMeshAsset());
	PoseableMesh->CopyPoseFromSkeletalComponent(PoseSource);

	if (GhostMaterial)
	{
		for (int32 i = 0; i < PoseableMesh->GetNumMaterials(); ++i)
		{
			PoseableMesh->SetMaterial(i, GhostMaterial);
		}
	}

	SetLifeSpan(Lifetime);
}


