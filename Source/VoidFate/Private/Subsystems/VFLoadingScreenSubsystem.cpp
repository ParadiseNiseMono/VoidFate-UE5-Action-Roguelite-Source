// Paradise NiseMono All Rights Reserved


#include "Subsystems/VFLoadingScreenSubsystem.h"

#include "PreLoadScreenManager.h"
#include "VFDebugHelper.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/VFLoadingScreenInterface.h"
#include "VoidFateSettings/VFLoadingScreenSettings.h"
#include "VFFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/GameModeBase.h"

bool UVFLoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}

	return false;
}

void UVFLoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::OnMapPreLoaded);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnMapPostLoaded);
}

void UVFLoadingScreenSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

UWorld* UVFLoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
	if (UGameInstance* OwningGameInstance = GetGameInstance())
	{
		return OwningGameInstance->GetWorld();
	}

	return nullptr;
}

void UVFLoadingScreenSubsystem::Tick(float DeltaTime)
{
	TryUpdateLoadingScreen();
}

ETickableTickType UVFLoadingScreenSubsystem::GetTickableTickType() const
{
	if (IsTemplate()) return ETickableTickType::Never;

	return ETickableTickType::Conditional;
}

bool UVFLoadingScreenSubsystem::IsTickable() const
{
	return GetGameInstance() && GetGameInstance()->GetGameViewportClient();
}

TStatId UVFLoadingScreenSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UVFLoadingScreenSubsystem, STATGROUP_Tickables);
}

void UVFLoadingScreenSubsystem::OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName)
{
	if (WorldContext.OwningGameInstance != GetGameInstance()) return;

	SetTickableTickType(ETickableTickType::Conditional);

	bIsCurrentLoadingScreen = true;

	TryUpdateLoadingScreen();
}

void UVFLoadingScreenSubsystem::OnMapPostLoaded(UWorld* LoadedWorld)
{
	if (LoadedWorld && LoadedWorld->GetGameInstance() == GetGameInstance())
	{
		bIsCurrentLoadingScreen = false;
	}
}

void UVFLoadingScreenSubsystem::TryUpdateLoadingScreen()
{
	//Check if there's any start up loading screen that current active.
	if (IsPreLoadScreenActive()) return;

	//Check if we should show the loading screen.
	if (ShouldShowLoadingScreen())
	{
		//Try display the loading screen here
		TryDisplayLoadingScreenIfNone();

		OnLoadingReasonUpdated.Broadcast(CurrentLoadingReason);
	}
	else
	{
		//Try to remove the current active loading screen
		TryRemoveLoadingScreen();

		HoldLoadingScreenStartUpTime = -1.f;

		//Notify the loading is completed
		NotifyLoadingScreenVisibilityChanged(false);

		//Disable the ticking
		SetTickableTickType(ETickableTickType::Never);
	}
}

bool UVFLoadingScreenSubsystem::IsPreLoadScreenActive() const
{
	if(FPreLoadScreenManager* PreLoadScreenManager = FPreLoadScreenManager::Get())
	{
		return PreLoadScreenManager->HasValidActivePreLoadScreen();
	}

	return false;
}

bool UVFLoadingScreenSubsystem::ShouldShowLoadingScreen()
{
	const UVFLoadingScreenSettings* LoadingScreenSettings = GetDefault<UVFLoadingScreenSettings>();
	
	if (GIsEditor && !LoadingScreenSettings->bShouldLoadingScreenInEditor) return false;

	//Check if the object in world need a loading screen
	if (CheckTheNeedToShowLoadingScreen())
	{
		GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = true;

		return true;
	}

	CurrentLoadingReason = UVFFunctionLibrary::GetLoadingScreenText("TextureStreamingKey").ToString();

	//There's no need to show the loading screen. Allow the world to be rendered to our viewport here.
	GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = false;

	const float CurrentTime = FPlatformTime::Seconds();
	if (HoldLoadingScreenStartUpTime < 0.f)
	{
		HoldLoadingScreenStartUpTime = CurrentTime;
	}

	const float ElapsedTime = CurrentTime - HoldLoadingScreenStartUpTime;

	if (ElapsedTime < LoadingScreenSettings->HoldLoadingScreenExtraSeconds)
	{
		return true;
	}

	return false;
}

bool UVFLoadingScreenSubsystem::CheckTheNeedToShowLoadingScreen()
{
	if (bIsCurrentLoadingScreen)
	{
		CurrentLoadingReason = UVFFunctionLibrary::GetLoadingScreenText("LoadingLevelKey").ToString();
		
		return true;
	}
	UWorld* OwningWorld = GetGameInstance()->GetWorld();

	if (!OwningWorld)
	{
		CurrentLoadingReason = UVFFunctionLibrary::GetLoadingScreenText("InitializingWorldKey").ToString();
		
		return true;
	}

	if (!OwningWorld->HasBegunPlay())
	{
		CurrentLoadingReason = UVFFunctionLibrary::GetLoadingScreenText("WorldNotBegunKey").ToString();

		return true;
	}

	if (!OwningWorld->GetFirstPlayerController())
	{
		CurrentLoadingReason = UVFFunctionLibrary::GetLoadingScreenText("PlayerControllerNotValidKey").ToString();

		return true;
	}

	APlayerController* PC = OwningWorld->GetFirstPlayerController();
	if (!PC)
	{
		CurrentLoadingReason = UVFFunctionLibrary::GetLoadingScreenText("PlayerControllerNotValidKey").ToString();
		return true;
	}

	if (!OwningWorld->GetGameState())
	{
		CurrentLoadingReason = UVFFunctionLibrary::GetLoadingScreenText("GameStateNotReadyKey").ToString();
		return true;
	}

	if (!PC->PlayerState)
	{
		CurrentLoadingReason = UVFFunctionLibrary::GetLoadingScreenText("PlayerStateNotReadyKey").ToString();
		return true;
	}

	APawn* PlayerPawn = PC->GetPawn();
	if (!PlayerPawn)
	{
		CurrentLoadingReason = UVFFunctionLibrary::GetLoadingScreenText("PlayerPawnNotReadyKey").ToString();
		return true;
	}
	
	return false;
}

void UVFLoadingScreenSubsystem::TryDisplayLoadingScreenIfNone()
{
	//If there is already active loading screen, return early if yes.
	if (CachedCachedLoadingScreenWidget) return;
	
	const UVFLoadingScreenSettings* LoadingScreenSettings = GetDefault<UVFLoadingScreenSettings>();
	
	TSubclassOf<UUserWidget> LoadingScreenClass = LoadingScreenSettings->GetLoadingScreenWidgetClassChecked();

	UUserWidget* CreatedWidget = UUserWidget::CreateWidgetInstance(*GetGameInstance(), LoadingScreenClass, NAME_None);
	
	check(CreatedWidget);
	
	CachedCachedLoadingScreenWidget = CreatedWidget->TakeWidget();

	GetGameInstance()->GetGameViewportClient()->AddViewportWidgetContent(
		CachedCachedLoadingScreenWidget.ToSharedRef(),
		1000
	);

	NotifyLoadingScreenVisibilityChanged(true);
}

void UVFLoadingScreenSubsystem::TryRemoveLoadingScreen()
{
	if (!CachedCachedLoadingScreenWidget) return;

	GetGameInstance()->GetGameViewportClient()->RemoveViewportWidgetContent(CachedCachedLoadingScreenWidget.ToSharedRef());

	CachedCachedLoadingScreenWidget.Reset();

}

void UVFLoadingScreenSubsystem::NotifyLoadingScreenVisibilityChanged(bool bIsVisible)
{
	UWorld* World = GetGameInstance()->GetWorld();
	if (!World) return;

	if (AGameModeBase* AuthGameMode = World->GetAuthGameMode())
	{
		if (AuthGameMode->Implements<UVFLoadingScreenInterface>())
		{
			if (bIsVisible)
			{
				IVFLoadingScreenInterface::Execute_OnLoadingScreenActivated(AuthGameMode);
			}
			else
			{
				IVFLoadingScreenInterface::Execute_OnLoadingScreenDeactivated(AuthGameMode);
			}
		}
	}

	for (ULocalPlayer* ExistingLocalPlayers : GetGameInstance()->GetLocalPlayers())
	{
		if (!ExistingLocalPlayers) continue;

		if (APlayerController* PC = ExistingLocalPlayers->GetPlayerController(GetGameInstance()->GetWorld()))
		{
			//Query if the player controller implements the interface. Call the function through interface to notify the loading status if yes.
			if (PC->Implements<UVFLoadingScreenInterface>())
			{
				if (bIsVisible)
				{
					IVFLoadingScreenInterface::Execute_OnLoadingScreenActivated(PC);
				}
				else
				{
					IVFLoadingScreenInterface::Execute_OnLoadingScreenDeactivated(PC);
				}
			}

			if (APawn* OwningPawn = PC->GetPawn())
			{
				if (OwningPawn->Implements<UVFLoadingScreenInterface>())
				{
					if (bIsVisible)
					{
						IVFLoadingScreenInterface::Execute_OnLoadingScreenActivated(OwningPawn);
					}
					else
					{
						IVFLoadingScreenInterface::Execute_OnLoadingScreenDeactivated(OwningPawn);
					}
				}
			}

			//The code for notifying other objects in the world goes here.
		}
	}
}
