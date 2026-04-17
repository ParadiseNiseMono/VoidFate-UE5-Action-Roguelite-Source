// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VFUIController.generated.h"

/**
 * 
 */
UCLASS()
class VOIDFATE_API AVFUIController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	//~ Begin APlayerController Interface
	virtual void OnPossess(APawn* InPawn) override;
	//~ End APlayerController Interface
};
