// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "VFTypes/VFEnumTypes.h"

/**
 * 用來倒數冷卻的LatentAction
 */
class VOIDFATE_API FVFCountDownAction : public FPendingLatentAction
{
public:
	FVFCountDownAction(float InTotalCountDownTime,float InUpdateInterval,float& InOutRemainingTime,EVFCountDownActionOutput& InCountDownOutput,const FLatentActionInfo& LatentInfo)
	: bNeedToCancel(false)
	, TotalCountDownTime(InTotalCountDownTime)
	, UpdateInterval(InUpdateInterval)
	, OutRemainingTime(InOutRemainingTime)
	, CountDownOutput(InCountDownOutput)
	, ExecutionFunction(LatentInfo.ExecutionFunction)
	, OutputLink(LatentInfo.Linkage)
	, CallbackTarget(LatentInfo.CallbackTarget)
	, ElapsedInterval(0.f)
	, ElapsedTimeSinceStart(0.f)
	{
	}
	//~ Begin FPendingLatentAction Interface
	virtual void UpdateOperation(FLatentResponse& Response) override;
	//~ End FPendingLatentAction Interface

	void CancelAction();

	// 用來強制重置倒數狀態與時間
	void ResetAction(float NewTotalCountDownTime, float NewUpdateInterval);
private:
	bool bNeedToCancel;
	float TotalCountDownTime;
	float UpdateInterval;
	float& OutRemainingTime;
	EVFCountDownActionOutput& CountDownOutput;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	float ElapsedInterval;
	float ElapsedTimeSinceStart;
};
