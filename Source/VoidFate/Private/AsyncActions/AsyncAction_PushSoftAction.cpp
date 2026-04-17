// Paradise NiseMono All Rights Reserved


#include "AsyncActions/AsyncAction_PushSoftAction.h"

#include "Subsystems/VFUISubsystem.h"

UAsyncAction_PushSoftAction* UAsyncAction_PushSoftAction::PushSoftWidget(const UObject* WorldContextObject,
                                                                         APlayerController* OwningPlayerController, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass,
                                                                         UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag InWidgetStackTag, bool bFocusOnNewlyPushedWidget)
{
	checkf(!InSoftWidgetClass.IsNull(), TEXT("InSoftWidgetClass is null"));

	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert))
		{
			UAsyncAction_PushSoftAction* Node = NewObject<UAsyncAction_PushSoftAction>();
			Node->CachedOwningWorld = World;
			Node->CachedOwingPC = OwningPlayerController;
			Node->CachedSoftWidgetClass = InSoftWidgetClass;
			Node->CachedWidgetStackTag = InWidgetStackTag;
			Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;

			Node->RegisterWithGameInstance(World);

			return Node;
		}
	}
	return nullptr;
}

void UAsyncAction_PushSoftAction::Activate()
{
	UVFUISubsystem* VFUISubsystem = UVFUISubsystem::Get(CachedOwningWorld.Get());

	VFUISubsystem->PushSoftWidgetToStackAsync(CachedWidgetStackTag, CachedSoftWidgetClass,
		[this](EAsyncPushWidgetState InPushState, UWidget_ActivatableBase* PushedWidget)
		{
			switch (InPushState)
			{
				case EAsyncPushWidgetState::AfterPush:
					PushedWidget->SetOwningPlayer(CachedOwingPC.Get());
					OnWidgetCreatedBeforePush.Broadcast(PushedWidget);
					
					break;
				case EAsyncPushWidgetState::OnCreatedBeforePush:
					AfterPush.Broadcast(PushedWidget);
					if (bCachedFocusOnNewlyPushedWidget)
					{
						if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusWidget())
						{
							WidgetToFocus->SetFocus();
						}
					}

					SetReadyToDestroy();
				
					break;
				
				default: ;
			}
		}
	);
}
