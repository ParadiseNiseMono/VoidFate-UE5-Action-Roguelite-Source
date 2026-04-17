// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/GhostTrail/VFGhostActor.h"
#include "GameplayTagContainer.h"
#include "VFPhantomBombActor.generated.h"

class UMaterialInterface;
class UGameplayEffect;
class UVFAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class VOIDFATE_API AVFPhantomBombActor : public AVFGhostActor
{
	GENERATED_BODY()
	
public:
	AVFPhantomBombActor();

	UFUNCTION(BlueprintCallable, Category = "VoidFate|PhantomBomb")
	void InitBomb();

	UFUNCTION(BlueprintCallable, Category = "VoidFate|PhantomBomb")
	void Explode();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PhantomBomb|Visual")
	UMaterialInterface* BombMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PhantomBomb|Combat")
	float DetonationTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PhantomBomb|Combat")
	float ExplosionRadius = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PhantomBomb|Combat")
	float ExplosionBaseDamage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PhantomBomb|Combat")
	bool bAutoExplode = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PhantomBomb|Debug")
	bool bShowDebugSphere = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PhantomBomb|Debug")
	float DebugSphereDuration = 5.0f;

	// 支援多種爆破效果的 GE 陣列
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PhantomBomb|Combat")
	TArray<TSubclassOf<UGameplayEffect>> ExplosionEffects;

	// 引爆時的特效/音效 Cue Tag
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PhantomBomb|Combat", meta = (Categories = "GameplayCue"))
	FGameplayTag ExplosionCueTag;

	// 範圍偵測過濾
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PhantomBomb|Combat")
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes;

	// Spawn時設定
	UPROPERTY(BlueprintReadOnly, Category = "PhantomBomb|Combat", meta = (ExposeOnSpawn = "true"))
	UVFAbilitySystemComponent* InstigatorVFASC;

	// Spawn時設定
	UPROPERTY(BlueprintReadOnly, Category = "PhantomBomb|Combat", meta = (ExposeOnSpawn = "true"))
	USkeletalMeshComponent* PoseSource;

private:
	FTimerHandle DetonationTimerHandle;
};
