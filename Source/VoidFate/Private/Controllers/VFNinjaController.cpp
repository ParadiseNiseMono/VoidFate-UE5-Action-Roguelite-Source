// Paradise NiseMono All Rights Reserved


#include "Controllers/VFNinjaController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "VFGameplayTags.h"
#include "Subsystems/VFUISubsystem.h"
#include "Widgets/VFHUDWidget.h"

AVFNinjaController::AVFNinjaController()
{
	NinjaTeamID = FGenericTeamId(0);
}

FGenericTeamId AVFNinjaController::GetGenericTeamId() const
{
	return NinjaTeamID;
}

void AVFNinjaController::BeginPlay()
{
	Super::BeginPlay();
}
