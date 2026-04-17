// Paradise NiseMono All Rights Reserved


#include "AI/BTTask_ActivateAbilityWithTarget.h"
#include "AbilitySystem/Abilities/VFGameplayAbility.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/VFAbilitySystemComponent.h"

UBTTask_ActivateAbilityWithTarget::UBTTask_ActivateAbilityWithTarget()
{
	NodeName = "Activate Ability By Event With Target";
}

EBTNodeResult::Type UBTTask_ActivateAbilityWithTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!TargetActor) return EBTNodeResult::Failed;

	UVFAbilitySystemComponent* ASC = Cast<UVFAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AIPawn));
	if (!ASC) return EBTNodeResult::Failed;

	bool bSuccess = ASC->TryActivateAbilityByEvent(AbilityEventTag, TargetActor);

	if (bSuccess)
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
