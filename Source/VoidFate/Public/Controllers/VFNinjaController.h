// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "VFNinjaController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPauseMenuOpenedDelegate, bool, bOpened);

class UVFHUDWidget;
/**
 * 
 */
UCLASS()
class VOIDFATE_API AVFNinjaController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AVFNinjaController();
	
	//~ Begin IGenericTeamAgentInterface Interface.
	virtual FGenericTeamId GetGenericTeamId() const override;
	//~ End IGenericTeamAgentInterface Interface.

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPauseMenuOpenedDelegate OnPauseMenuOpened;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSoftClassPtr<UVFHUDWidget> SoftMainHUDClass;

	UPROPERTY()
	TObjectPtr<UVFHUDWidget> MainHUDInstance;

	FGenericTeamId NinjaTeamID;
};
