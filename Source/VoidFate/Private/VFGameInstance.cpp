// Paradise NiseMono All Rights Reserved


#include "VFGameInstance.h"

#include "VoidFateSettings/VFGameUserSettings.h"


TSoftObjectPtr<UWorld> UVFGameInstance::GetGameLevelByTag(FGameplayTag InTag) const
{
	for (const FVFGameLevelSet& GameLevelSet : GameLevelSets)
	{
		if(!GameLevelSet.IsValid()) continue;

		if (GameLevelSet.LevelTag == InTag)
		{
			return GameLevelSet.Level;
		}
	}

	return TSoftObjectPtr<UWorld>();
}

void UVFGameInstance::Init()
{
	Super::Init();
}

void UVFGameInstance::OnStart()
{
	Super::OnStart();

	if (UVFGameUserSettings* Settings = UVFGameUserSettings::Get())
	{
		Settings->RunFirstLaunchSetup();
	}
}
