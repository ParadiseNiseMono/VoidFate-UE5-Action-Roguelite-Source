// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "VFDeveloperSettings.generated.h"

class UWidget_ActivatableBase;
/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "VoidFate UI Settings"))
class VOIDFATE_API UVFDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Widget Reference" ,meta=(Categories = "Frontend.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UWidget_ActivatableBase>> FrontWidgetBase;

	UPROPERTY(Config, EditAnywhere, Category = "Options Image Reference" ,meta=(Categories = "Frontend.Image"))
	TMap<FGameplayTag, TSoftObjectPtr<UTexture2D>> OptionsScreenSoftImageMap;

	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath MasterSoundClass;
 
	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath MusicSoundClass;
 
	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath SoundFXSoundClass;

	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath UIAudioSoundClass;

	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath VoiceSoundClass;
 
	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundMix"))
	FSoftObjectPath DefaultSoundMix;

	UPROPERTY(Config, EditAnywhere, Category = "BGM")
	TSoftObjectPtr<USoundBase> EasyModeBGM;

	UPROPERTY(Config, EditAnywhere, Category = "BGM")
	TSoftObjectPtr<USoundBase> NormalModeBGM;

	UPROPERTY(Config, EditAnywhere, Category = "BGM")
	TSoftObjectPtr<USoundBase> HardModeBGM;

	UPROPERTY(Config, EditAnywhere, Category = "BGM")
	TSoftObjectPtr<USoundBase> VeryHardModeBGM;

	UPROPERTY(Config, EditAnywhere, Category = "BGM")
	TSoftObjectPtr<USoundBase> MainMenuBGM;
	
	UPROPERTY(Config, EditAnywhere, Category = "String Table")
	TSoftObjectPtr<UStringTable> OptionsScreenDescriptionStringTable;

	UPROPERTY(Config, EditAnywhere, Category = "String Table")
	TSoftObjectPtr<UStringTable> OptionsScreenTitleStringTable;

	UPROPERTY(Config, EditAnywhere, Category = "String Table")
	TSoftObjectPtr<UStringTable> ModalScreenTitleStringTable;

	UPROPERTY(Config, EditAnywhere, Category = "String Table")
	TSoftObjectPtr<UStringTable> PlayerStateScreenStringTable;

	UPROPERTY(Config, EditAnywhere, Category = "String Table")
	TSoftObjectPtr<UStringTable> LoadingScreenStringTable;
};
