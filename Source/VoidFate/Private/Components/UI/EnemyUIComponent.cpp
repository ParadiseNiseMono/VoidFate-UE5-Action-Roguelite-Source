// Paradise NiseMono All Rights Reserved


#include "Components/UI/EnemyUIComponent.h"

#include "Widgets/VFUserWidgetBase.h"

void UEnemyUIComponent::RegisterEnemyDrawnWidget(UVFUserWidgetBase* InWidgetToRegister)
{
	EnemyDrawnWidgets.Add(InWidgetToRegister);
}

void UEnemyUIComponent::RemoveEnemyDrawnWidgetsIfAny()
{
	if (EnemyDrawnWidgets.IsEmpty()) return;

	for (UVFUserWidgetBase* Widget : EnemyDrawnWidgets)
	{
		if (Widget)
		{
			Widget->RemoveFromParent();
			Widget->DeactivateWidget();
		}
	}

	EnemyDrawnWidgets.Empty();
}
