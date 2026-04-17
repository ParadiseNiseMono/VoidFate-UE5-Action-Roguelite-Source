// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/VFBaseAnimInstance.h"
#include "VFNinjaLinkedAnimLayer.generated.h"

class UVFNinjaAnimInstance;
/**
 * 
 */
UCLASS()
class VOIDFATE_API UVFNinjaLinkedAnimLayer : public UVFBaseAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	UVFNinjaAnimInstance* GetNinjaAnimInstance() const;
};
