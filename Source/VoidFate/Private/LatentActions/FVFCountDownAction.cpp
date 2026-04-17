// Paradise NiseMono All Rights Reserved


#include "LatentActions/FVFCountDownAction.h"

void FVFCountDownAction::UpdateOperation(FLatentResponse& Response)
{
	UObject* TargetObject = CallbackTarget.Get();
	if (TargetObject && TargetObject->GetWorld())
	{
		if (TargetObject->GetWorld()->IsPaused())
		{
			return;
		}
	}

	if (bNeedToCancel)
	{
		CountDownOutput = EVFCountDownActionOutput::Cancelled;

		Response.FinishAndTriggerIf(true,ExecutionFunction,OutputLink,CallbackTarget);

		return;
	}

	if (ElapsedTimeSinceStart >= TotalCountDownTime)
	{
		CountDownOutput = EVFCountDownActionOutput::Completed;

		Response.FinishAndTriggerIf(true,ExecutionFunction,OutputLink,CallbackTarget);

		return;
	}

	if (ElapsedInterval < UpdateInterval)
	{
		ElapsedInterval += Response.ElapsedTime();
	}
	else
	{
		ElapsedTimeSinceStart += UpdateInterval > 0.f? UpdateInterval : Response.ElapsedTime();

		OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart;

		CountDownOutput = EVFCountDownActionOutput::Updated;

		Response.TriggerLink(ExecutionFunction,OutputLink,CallbackTarget);
		  
		ElapsedInterval = 0.f;
	}
}

void FVFCountDownAction::CancelAction()
{
	bNeedToCancel = true;
}

void FVFCountDownAction::ResetAction(float NewTotalCountDownTime, float NewUpdateInterval)
{
	TotalCountDownTime = NewTotalCountDownTime;
	UpdateInterval = NewUpdateInterval;

	ElapsedTimeSinceStart = 0.f;
	ElapsedInterval = 0.f;

	bNeedToCancel = false;
}
