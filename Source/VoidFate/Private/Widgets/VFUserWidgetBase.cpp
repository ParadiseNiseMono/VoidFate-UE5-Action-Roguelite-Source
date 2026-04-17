// Paradise NiseMono All Rights Reserved


#include "Widgets/VFUserWidgetBase.h"

#include "Interfaces/PawnUIInterface.h"

void UVFUserWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (UNinjaUIComponent* NinjaUIComponent = PawnUIInterface->GetNinjaUIComponent())
		{
			BP_OnOwningNinjaUIComponentInitialized(NinjaUIComponent);
		}
	}
}

void UVFUserWidgetBase::InitEnemyCreatedWidget(AActor* OwningEnemyActor)
{
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(OwningEnemyActor))
	{
		UEnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent();

		checkf(EnemyUIComponent, TEXT("Failed to get an EnemyUIComponent form %s"), *OwningEnemyActor->GetActorNameOrLabel());

		BP_OnOwningEnemyUIComponentInitialized(EnemyUIComponent);
	}
}
