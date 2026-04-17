// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VFGhostActor.generated.h"

class UPoseableMeshComponent;

UCLASS()
class VOIDFATE_API AVFGhostActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AVFGhostActor();

	void InitGhost(USkeletalMeshComponent* PoseSource, UMaterialInterface* GhostMaterial, float Lifetime);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ghost")
	UPoseableMeshComponent* PoseableMesh;
};
