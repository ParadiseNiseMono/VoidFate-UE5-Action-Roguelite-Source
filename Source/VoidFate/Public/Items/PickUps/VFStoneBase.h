// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUps/VFPickUpBase.h"
#include "VFStoneBase.generated.h"

class UGameplayEffect;
class UVFAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class VOIDFATE_API AVFStoneBase : public AVFPickUpBase
{
	GENERATED_BODY()

public:
	void PickUpStones(UVFAbilitySystemComponent* AbilitySystemComponent,int32 ApplyLevel);
	
protected:
	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Stone Pickuped"))
	void BP_OnStonePickUped();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StoneGameplayEffectClass;
};
