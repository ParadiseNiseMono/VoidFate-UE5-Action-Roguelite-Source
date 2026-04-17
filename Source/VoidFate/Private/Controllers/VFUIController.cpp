// Paradise NiseMono All Rights Reserved


#include "Controllers/VFUIController.h"

#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "VoidFateSettings/VFGameUserSettings.h"

void AVFUIController::BeginPlay()
{
	Super::BeginPlay();

	UVFGameUserSettings* GameUserSettings = UVFGameUserSettings::Get();

	GameUserSettings->SetOverallVolume(UVFGameUserSettings::Get()->GetOverallVolume());
	GameUserSettings->SetMusicVolume(UVFGameUserSettings::Get()->GetMusicVolume());
	GameUserSettings->SetSoundFXVolume(UVFGameUserSettings::Get()->GetSoundFXVolume());
	GameUserSettings->SetUIAudioVolume(UVFGameUserSettings::Get()->GetUIAudioVolume());
	GameUserSettings->SetVoiceVolume(UVFGameUserSettings::Get()->GetVoiceVolume());
	GameUserSettings->SetAllowBackgroundAudio(UVFGameUserSettings::Get()->GetAllowBackgroundAudio());
	GameUserSettings->SetUseHDRAudioMode(UVFGameUserSettings::Get()->GetUseHDRAudioMode());
}

void AVFUIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	TArray<AActor*> FoundCameras;

	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), FName("Default"), FoundCameras);

	if (!FoundCameras.IsEmpty())
	{
		SetViewTarget(FoundCameras[0]);
	}

	UVFGameUserSettings* GameUserSettings = UVFGameUserSettings::Get();

	if (GameUserSettings->GetLastCPUBenchmarkResult() == -1 || GameUserSettings->GetLastGPUBenchmarkResult() == -1)
	{
		GameUserSettings->RunHardwareBenchmark();
		GameUserSettings->ApplyHardwareBenchmarkResults();
	}
}
