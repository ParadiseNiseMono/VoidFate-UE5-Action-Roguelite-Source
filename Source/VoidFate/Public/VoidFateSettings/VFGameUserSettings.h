// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "VFTypes/VFEnumTypes.h"
#include "VFGameUserSettings.generated.h"

DECLARE_DELEGATE_OneParam(FOnFloatValueChanged, float);
DECLARE_DELEGATE_OneParam(FOnBoolValueChanged, bool);
/**
 * 
 */
UCLASS()
class VOIDFATE_API UVFGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UVFGameUserSettings();
	
	static UVFGameUserSettings* Get();

	FOnFloatValueChanged OnCameraZoomDistanceChanged;

	FOnBoolValueChanged OnLockVerticalCameraChanged;

	//***** Gameplay Collection Tab *****//
	UFUNCTION(BlueprintCallable, Category = "Void|Settings")
	EVFGameDifficulty GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }

	UFUNCTION()
	void SetCurrentGameDifficulty(EVFGameDifficulty InNewDifficulty) { CurrentGameDifficulty = InNewDifficulty; }

	UFUNCTION()
	float GetCameraZoomDistance() const { return CameraZoomDistance; }

	UFUNCTION()
	void SetCameraZoomDistance(float InDistance);

	UFUNCTION(BlueprintCallable, Category = "Void|Settings")
	bool GetLockVerticalCamera() const { return bLockVerticalCamera; }

	UFUNCTION(BlueprintCallable, Category = "Void|Settings")
	void SetLockVerticalCamera(bool bInLock);
	//***** Gameplay Collection Tab *****//

	//***** Audio Collection Tab *****//
	UFUNCTION()
	float GetOverallVolume() const {return OverallVolume;}

	UFUNCTION()
	void SetOverallVolume(const float InVolume);

	UFUNCTION()
	float GetMusicVolume() const {return MusicVolume;}

	UFUNCTION()
	void SetMusicVolume(const float InVolume);

	UFUNCTION()
	float GetSoundFXVolume() const {return SoundFXVolume;}

	UFUNCTION()
	void SetSoundFXVolume(const float InVolume);

	UFUNCTION()
	float GetUIAudioVolume() const {return UIAudioVolume;}

	UFUNCTION()
	void SetUIAudioVolume(const float InVolume);

	UFUNCTION()
	float GetVoiceVolume() const {return VoiceVolume;}

	UFUNCTION()
	void SetVoiceVolume(const float InVolume);

	UFUNCTION()
	bool GetAllowBackgroundAudio() const {return bAllowBackgroundAudio;}

	UFUNCTION()
	void SetAllowBackgroundAudio(bool bIsAllowed);

	UFUNCTION()
	bool GetUseHDRAudioMode() const {return bUseHDRAudioMode;}

	UFUNCTION()
	void SetUseHDRAudioMode(bool bIsAllowed);
	//***** Audio Collection Tab *****//

	//***** Video Collection Tab *****//
	UFUNCTION()
	float GetCurrentDisplayGamma() const;

	UFUNCTION()
	void SetCurrentDisplayGamma(float InNewGamma);
	//***** Video Collection Tab *****//

	//***** Language Collection Tab *****//
	UFUNCTION()
	FString GetCurrentLanguage() const;

	UFUNCTION()
	void SetCurrentLanguage(const FString& InNewLanguage);
	//***** Language Collection Tab *****//

	UFUNCTION(BlueprintCallable)
	void RunFirstLaunchSetup();

private:
	/** 通用的音量設置處理函數 */
	void ApplyVolumeSetting(USoundClass* SoundClass, float NewVolume, float& TargetMember);
	
	//***** Gameplay Collection Tab *****//
	UPROPERTY(Config)
	EVFGameDifficulty CurrentGameDifficulty;

	UPROPERTY(Config)
	float CameraZoomDistance;

	UPROPERTY(Config)
	bool bLockVerticalCamera;
	//***** Gameplay Collection Tab *****//

	//***** Audio Collection Tab *****//
	UPROPERTY(Config)
	float OverallVolume;

	UPROPERTY(Config)
	float MusicVolume;

	UPROPERTY(Config)
	float SoundFXVolume;

	UPROPERTY(Config)
	float UIAudioVolume;

	UPROPERTY(Config)
	float VoiceVolume;

	UPROPERTY(Config)
	bool bAllowBackgroundAudio;

	UPROPERTY(Config)
	bool bUseHDRAudioMode;
	//***** Audio Collection Tab *****//

	//***** Language Collection Tab *****//
	UPROPERTY(Config)
	FString CurrentLanguage;
	//***** Language Collection Tab *****//

	UPROPERTY(Config)
	bool bIsFirstLaunch;
};
