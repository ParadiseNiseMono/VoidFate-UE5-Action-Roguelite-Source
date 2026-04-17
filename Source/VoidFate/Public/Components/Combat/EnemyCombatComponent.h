// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "EnemyCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class VOIDFATE_API UEnemyCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	//~ Begin UPawnCombatComponent Interface
	virtual void OnHitTargetActor(AActor* HitActor, const FHitResult& ImpactResult) override;
	//~ End UPawnCombatComponent Interface
};
