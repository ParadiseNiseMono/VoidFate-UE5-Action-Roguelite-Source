// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_PlayCinematicCamera.generated.h"

/**
 * 專門負責管理處決運鏡生命週期的 Task
 * 優點：當 Ability 結束或被取消時，會自動復原攝影機，防止鏡頭鎖死
 */
UCLASS()
class VOIDFATE_API UAbilityTask_PlayCinematicCamera : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Void|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAbilityTask_PlayCinematicCamera* PlayCinematicCamera(
		UGameplayAbility* OwningAbility,
		AActor* Attacker,
		AActor* Victim,
		FVector CameraOffset,
		float BlendDuration = 0.2f
	);

protected:
	//~Begin UGameplayTask interface
	virtual void Activate() override;
	//~End UGameplayTask interface

	//~Begin UAbilityTask interface
	virtual void OnDestroy(bool bInOwnerFinished) override;
	//~End UAbilityTask interface

private:
	UPROPERTY()
	AActor* CachedAttacker;

	UPROPERTY()
	AActor* CachedVictim;

	FVector CachedOffset;
	float CachedBlendDuration;
};
