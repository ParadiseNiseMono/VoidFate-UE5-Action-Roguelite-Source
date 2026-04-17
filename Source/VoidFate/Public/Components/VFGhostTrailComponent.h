// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VFGhostTrailComponent.generated.h"


class UPoseableMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOIDFATE_API UVFGhostTrailComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVFGhostTrailComponent();

	// 開始產生殘影
	UFUNCTION(BlueprintCallable, Category = "Void|VFX")
	void StartGhostTrail();

	// 停止產生
	UFUNCTION(BlueprintCallable, Category = "Void|VFX")
	void StopGhostTrail();

protected:
	virtual void BeginPlay() override;

	// 殘影材質
	UPROPERTY(EditDefaultsOnly, Category = "Ghost Settings")
	UMaterialInterface* GhostMaterial;

	// 產生頻率 (每秒產生幾個殘影)
	UPROPERTY(EditDefaultsOnly, Category = "Ghost Settings")
	float SpawnRate = 10.0f; // 數值越高越密集

	// 殘影存活時間 (秒)
	UPROPERTY(EditDefaultsOnly, Category = "Ghost Settings")
	float GhostLifetime = 0.5f;

private:	
	// 生成一個殘影
	void SpawnGhost();

	// 銷毀一個殘影 (給 Timer 用)
	UFUNCTION()
	void DestroyGhost(UPoseableMeshComponent* MeshToDestroy);

	FTimerHandle SpawnTimerHandle;

	// 暫存角色的 Skeletal Mesh 引用
	UPROPERTY()
	USkeletalMeshComponent* OwnerMesh;

		
};
