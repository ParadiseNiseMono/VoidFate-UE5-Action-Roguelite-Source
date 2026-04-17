// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VFLoadingScreenInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI)
class UVFLoadingScreenInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Handle loading screen notify
 */
class VOIDFATE_API IVFLoadingScreenInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void OnLoadingScreenActivated();
	virtual void OnLoadingScreenActivated_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnLoadingScreenDeactivated();
	virtual void OnLoadingScreenDeactivated_Implementation();
};
