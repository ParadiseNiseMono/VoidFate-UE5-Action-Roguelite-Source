// Paradise NiseMono All Rights Reserved


#include "GameModes/VFBaseGameMode.h"

#include "VoidFateSettings/VFGameUserSettings.h"

AVFBaseGameMode::AVFBaseGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AVFBaseGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UVFGameUserSettings* GameUserSettings = UVFGameUserSettings::Get();
	CurrentGameDifficulty = GameUserSettings->GetCurrentGameDifficulty();
}