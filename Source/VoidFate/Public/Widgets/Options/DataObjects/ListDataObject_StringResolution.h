// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "ListDataObject_StringResolution.generated.h"

/**
 * 
 */
UCLASS()
class VOIDFATE_API UListDataObject_StringResolution : public UListDataObject_String
{
	GENERATED_BODY()

public:
	void InitResolutionValues();

protected:
	// ~Begin UListDataObject_Base Interface
	virtual void OnDataObjectInitialized() override;
	// ~End UListDataObject_Base Interface

private:
	FString ResToValueString(const FIntPoint& InResolution);
	FText ResToDisplayText(const FIntPoint& InResolution);

	FString MaximumAllowedResolution;

public:
	FORCEINLINE FString GetMaximumAllowedResolution() { return MaximumAllowedResolution; }
};
