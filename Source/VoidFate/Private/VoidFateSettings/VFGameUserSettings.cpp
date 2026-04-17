// Paradise NiseMono All Rights Reserved


#include "VoidFateSettings/VFGameUserSettings.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "VoidFateSettings/VFDeveloperSettings.h"
#include "Kismet/KismetInternationalizationLibrary.h"

UVFGameUserSettings::UVFGameUserSettings()
	:CameraZoomDistance(0.5f), bLockVerticalCamera(false), OverallVolume(1.f), MusicVolume(0.6f), SoundFXVolume(1.f), UIAudioVolume(1.f), VoiceVolume(1.f), bAllowBackgroundAudio(false), bUseHDRAudioMode(false)
{
	FrameRateLimit = 0.f;
	bIsFirstLaunch = true;
}

UVFGameUserSettings* UVFGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UVFGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}

void UVFGameUserSettings::SetCameraZoomDistance(float InDistance)
{
	CameraZoomDistance = FMath::Clamp(InDistance, 0.0f, 1.0f);

	OnCameraZoomDistanceChanged.ExecuteIfBound(CameraZoomDistance);
}

void UVFGameUserSettings::SetLockVerticalCamera(bool bIsLocked)
{
	bLockVerticalCamera = bIsLocked;

	OnLockVerticalCameraChanged.ExecuteIfBound(bIsLocked);
	
	SaveSettings();
}

void UVFGameUserSettings::SetOverallVolume(const float InVolume)
{
	const UVFDeveloperSettings* VFDeveloperSettings = GetDefault<UVFDeveloperSettings>();
	USoundClass* OverallSoundClass = nullptr;
	if (UObject* LoadedObject = VFDeveloperSettings->MasterSoundClass.TryLoad())
	{
		OverallSoundClass = CastChecked<USoundClass>(LoadedObject);
	}
	
	ApplyVolumeSetting(OverallSoundClass, InVolume, OverallVolume);
}

void UVFGameUserSettings::SetMusicVolume(const float InVolume)
{
	const UVFDeveloperSettings* VFDeveloperSettings = GetDefault<UVFDeveloperSettings>();
	USoundClass* MusicSoundClass = nullptr;
	if (UObject* LoadedObject = VFDeveloperSettings->MusicSoundClass.TryLoad())
	{
		MusicSoundClass = CastChecked<USoundClass>(LoadedObject);
	}
	
	ApplyVolumeSetting(MusicSoundClass, InVolume, MusicVolume);
}

void UVFGameUserSettings::SetSoundFXVolume(const float InVolume)
{
	const UVFDeveloperSettings* VFDeveloperSettings = GetDefault<UVFDeveloperSettings>();
	USoundClass* SoundFXSoundClass = nullptr;
	if (UObject* LoadedObject = VFDeveloperSettings->SoundFXSoundClass.TryLoad())
	{
		SoundFXSoundClass = CastChecked<USoundClass>(LoadedObject);
	}
	
	ApplyVolumeSetting(SoundFXSoundClass, InVolume, SoundFXVolume);
}

void UVFGameUserSettings::SetUIAudioVolume(const float InVolume)
{
	const UVFDeveloperSettings* VFDeveloperSettings = GetDefault<UVFDeveloperSettings>();
	USoundClass* UIAudioSoundClass = nullptr;
	if (UObject* LoadedObject = VFDeveloperSettings->UIAudioSoundClass.TryLoad())
	{
		UIAudioSoundClass = CastChecked<USoundClass>(LoadedObject);
	}
	
	ApplyVolumeSetting(UIAudioSoundClass, InVolume, UIAudioVolume);
}

void UVFGameUserSettings::SetVoiceVolume(const float InVolume)
{
	const UVFDeveloperSettings* VFDeveloperSettings = GetDefault<UVFDeveloperSettings>();
	USoundClass* VoiceSoundClass = nullptr;
	if (UObject* LoadedObject = VFDeveloperSettings->VoiceSoundClass.TryLoad())
	{
		VoiceSoundClass = CastChecked<USoundClass>(LoadedObject);
	}
	
	ApplyVolumeSetting(VoiceSoundClass, InVolume, VoiceVolume);
}

void UVFGameUserSettings::SetAllowBackgroundAudio(bool bIsAllowed)
{
	bAllowBackgroundAudio = bIsAllowed;
}

void UVFGameUserSettings::SetUseHDRAudioMode(bool bIsAllowed)
{
	bUseHDRAudioMode = bIsAllowed;
}

float UVFGameUserSettings::GetCurrentDisplayGamma() const
{
	if (GEngine)
	{
		return GEngine->GetDisplayGamma();
	}
	return 0.f;
}

void UVFGameUserSettings::SetCurrentDisplayGamma(float InNewGamma)
{
	if (GEngine)
	{
		GEngine->DisplayGamma = InNewGamma;
	}
}

FString UVFGameUserSettings::GetCurrentLanguage() const
{
	if (CurrentLanguage.IsEmpty())
	{
		FString SystemCulture = UKismetInternationalizationLibrary::GetCurrentCulture();


		if (SystemCulture.StartsWith(TEXT("ja")) || SystemCulture.StartsWith(TEXT("ja-JP")))
		{
			return TEXT("ja-JP");
		}

		if (SystemCulture.StartsWith(TEXT("zh")))
		{
			// 如果系統代碼包含 Hans (簡化字) 或是 CN (中國大陸) / SG (新加坡)
			if (SystemCulture.Contains(TEXT("Hans")) || SystemCulture.Contains(TEXT("CN")) || SystemCulture.Contains(TEXT("SG")))
			{
				return TEXT("zh-Hans-CN");
			}

			return TEXT("zh-Hant-TW");
		}
		return TEXT("en");
	}

	return CurrentLanguage;
}

void UVFGameUserSettings::SetCurrentLanguage(const FString& InNewLanguage)
{
	if (CurrentLanguage == InNewLanguage) return;

	CurrentLanguage = InNewLanguage;

	UKismetInternationalizationLibrary::SetCurrentCulture(CurrentLanguage, true);
}

void UVFGameUserSettings::RunFirstLaunchSetup()
{
	if (bIsFirstLaunch)
	{
		RunHardwareBenchmark();
		ApplyHardwareBenchmarkResults();

		if (GetOverallScalabilityLevel() > 2) SetOverallScalabilityLevel(2);
		if (GetViewDistanceQuality() > 2) SetViewDistanceQuality(2);
		if (GetAntiAliasingQuality() > 2) SetAntiAliasingQuality(2);
		if (GetShadowQuality() > 2) SetShadowQuality(2);
		if (GetGlobalIlluminationQuality() > 2) SetGlobalIlluminationQuality(2);
		if (GetReflectionQuality() > 2) SetReflectionQuality(2);
		if (GetPostProcessingQuality() > 2) SetPostProcessingQuality(2);
		if (GetTextureQuality() > 2) SetTextureQuality(2);
		if (GetVisualEffectQuality() > 2) SetVisualEffectQuality(2);
		if (GetFoliageQuality() > 2) SetFoliageQuality(2);
		if (GetShadingQuality() > 2) SetShadingQuality(2);
		if (GetResolutionScaleNormalized() > 0.7f) SetResolutionScaleNormalized(0.7f);
		
		FIntPoint DesktopRes = GetDesktopResolution();
		SetScreenResolution(DesktopRes);
		SetFullscreenMode(EWindowMode::WindowedFullscreen);

		CurrentLanguage = GetCurrentLanguage();

		SetLockVerticalCamera(false);

		SetVSyncEnabled(false);

		bIsFirstLaunch = false;

		ApplySettings(false);
		ApplyNonResolutionSettings();
		ApplyResolutionSettings(false);
		SaveSettings();
	}
}


void UVFGameUserSettings::ApplyVolumeSetting(USoundClass* SoundClass, float NewVolume,
                                             float& TargetMember)
{
	UWorld* World = GEngine ? GEngine->GetCurrentPlayWorld() : nullptr;
	const UVFDeveloperSettings* Settings = GetDefault<UVFDeveloperSettings>();
    
	if (!World || !Settings) return;
	
	USoundMix* DefaultSoundMix = nullptr;
	if (UObject* LoadedObject = Settings->DefaultSoundMix.TryLoad())
	{
		DefaultSoundMix = CastChecked<USoundMix>(LoadedObject);
	}

	if (!SoundClass || !DefaultSoundMix) return;
	
	TargetMember = NewVolume;

	UGameplayStatics::SetSoundMixClassOverride(
		World,
		DefaultSoundMix,
		SoundClass,
		TargetMember,
		1.0f,   
		0.2f,   
		true  
	);
	
	UGameplayStatics::PushSoundMixModifier(World, DefaultSoundMix);
}
