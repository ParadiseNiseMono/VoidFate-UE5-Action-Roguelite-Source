// Paradise NiseMono All Rights Reserved


#include "GameModes/VFTutorialGameMode.h"

AVFTutorialGameMode::AVFTutorialGameMode()
{
	CurrentPhase = EVFTutorialPhase::None;
}

void AVFTutorialGameMode::SetTutorialPhase(EVFTutorialPhase NewPhase)
{
	if (CurrentPhase == NewPhase)
	{
		return;
	}

	EVFTutorialPhase OldPhase = CurrentPhase;
	CurrentPhase = NewPhase;

	OnTutorialPhaseChanged.Broadcast(OldPhase, CurrentPhase);

	UE_LOG(LogTemp, Warning, TEXT("Tutorial Phase Changed: %d -> %d"), (int32)OldPhase, (int32)CurrentPhase);
}

void AVFTutorialGameMode::AdvanceToNextPhase()
{
	uint8 NextPhaseIndex = static_cast<uint8>(CurrentPhase) + 1;

	if (NextPhaseIndex < static_cast<uint8>(EVFTutorialPhase::Max))
	{
		SetTutorialPhase(static_cast<EVFTutorialPhase>(NextPhaseIndex));
	}
}

void AVFTutorialGameMode::IncrementPerfectDodgeCount()
{
	PerfectDodgeTimes++;
	OnPerfectDodgeCountChanged.Broadcast(PerfectDodgeTimes);
	UE_LOG(LogTemp, Warning, TEXT("Perfect Dodge Count Incremented: %d"), PerfectDodgeTimes);
}

void AVFTutorialGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetTutorialPhase(EVFTutorialPhase::Movement);
}
