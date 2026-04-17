// Paradise NiseMono All Rights Reserved


#include "Subsystems/VFBGMSubsystem.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameModes/VFBaseGameMode.h"
#include "VFFunctionLibrary.h"
#include "VoidFateSettings/VFDeveloperSettings.h"

void UVFBGMSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CurrentBGMComponent = nullptr;
	FadingBGMComponent = nullptr;
}

void UVFBGMSubsystem::Deinitialize()
{
	if (CurrentBGMComponent)
	{
		CurrentBGMComponent->Stop();
		CurrentBGMComponent = nullptr;
	}
	if (FadingBGMComponent)
	{
		FadingBGMComponent->Stop();
		FadingBGMComponent = nullptr;
	}

	Super::Deinitialize();
}

void UVFBGMSubsystem::PlayBGM(USoundBase* NewBGM, float FadeInTime, float FadeOutTime)
{
	if (!NewBGM)
	{
		return;
	}

	if (CurrentBGMComponent && CurrentBGMComponent->GetSound() == NewBGM && CurrentBGMComponent->IsPlaying())
	{
		return;
	}

	if (CurrentBGMComponent)
	{
		FadingBGMComponent = CurrentBGMComponent;

		FadingBGMComponent->FadeOut(FadeOutTime, 0.0f);
	}

	CurrentBGMComponent = UGameplayStatics::CreateSound2D(
		GetWorld(),
		NewBGM,
		1.0f,         // 這是基礎音量倍率，FadeIn 會以此為基準從 0 往上爬
		1.0f,
		0.0f,
		nullptr,
		true,         
		false         
	);

	if (CurrentBGMComponent)
	{
		CurrentBGMComponent->FadeIn(FadeInTime, 1.0f);
	}
}

void UVFBGMSubsystem::StopBGM(float FadeOutTime)
{
	if (CurrentBGMComponent)
	{
		CurrentBGMComponent->FadeOut(FadeOutTime, 0.0f);
		CurrentBGMComponent = nullptr;
	}
}

void UVFBGMSubsystem::PlayGameModeBGM()
{
	UWorld* World = GetWorld();
	if (!World) return;

	AVFBaseGameMode* GameMode = Cast<AVFBaseGameMode>(World->GetAuthGameMode());
	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("BGM切換失敗: 無法取得有效的 GameMode，可能是呼叫時機過早。"));
		return;
	}

	EVFGameDifficulty CurrentDifficulty = GameMode->GetCurrentGameDifficulty();
	USoundBase* BGMToPlay = UVFFunctionLibrary::GetBGMByGameDifficulty(CurrentDifficulty);

	if (BGMToPlay)
	{
		PlayBGM(BGMToPlay);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BGM切換失敗: GetBGMByGameDifficulty 回傳了 nullptr，請檢查 Project Settings 是否有設定該難度的音樂！"));
	}
}

void UVFBGMSubsystem::PlayMainMenuBGM()
{
	const UVFDeveloperSettings* Settings = GetDefault<UVFDeveloperSettings>();
	if (!Settings) return;

	PlayBGM(Settings->MainMenuBGM.LoadSynchronous());
}
