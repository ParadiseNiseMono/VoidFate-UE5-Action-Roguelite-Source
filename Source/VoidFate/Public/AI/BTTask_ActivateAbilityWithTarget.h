// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ActivateAbilityWithTarget.generated.h"

/**
 * 
 */
UCLASS()
class VOIDFATE_API UBTTask_ActivateAbilityWithTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ActivateAbilityWithTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "VoidFate|Ability")
	FGameplayTag AbilityEventTag;

	UPROPERTY(EditAnywhere, Category = "VoidFate|Ability")
	FBlackboardKeySelector TargetKey;
};
