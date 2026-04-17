// Paradise NiseMono All Rights Reserved


#include "Widgets/Options/OptionsDataRegistry.h"

#include <gsl/pointers>

#include "VFFunctionLibrary.h"
#include "VFGameplayTags.h"
#include "VoidFateSettings/VFGameUserSettings.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"
#include "Internationalization/StringTableRegistry.h"
#include "EnhancedInputSubsystems.h"
#include "VFDebugHelper.h"
#include "VFGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"
#include "VFTypes/VFEnumTypes.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UVFGameUserSettings, SetterOrGetterFuncName))

/*#define GET_DESCRIPTION(InKey) LOCTABLE("/Game/UI/StringTable/ST_OptionsScreenDescription.ST_OptionsScreenDescription", InKey)*/

#define GET_DESCRIPTION(InKey) LOCTABLE("/Game/UI/StringTable/ST_OptionsScreenDescription.ST_OptionsScreenDescription", InKey)

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab(InOwningLocalPlayer);
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab(InOwningLocalPlayer);
	InitLanguageCollectionTab();
	
}

TArray<UListDataObject_Base*> UOptionsDataRegistry::GetListSourceItemsBySelectedTabID(
	const FName& InSelectedTabID) const
{
	UListDataObject_Collection* const* FoundTabCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate(
		[InSelectedTabID](UListDataObject_Collection* AvailableTabCollection)
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
	);

	checkf(FoundTabCollectionPtr, TEXT("No valid tab found under the ID : "), *InSelectedTabID.ToString());

	UListDataObject_Collection* FoundTabCollection = *FoundTabCollectionPtr;

	TArray<UListDataObject_Base*> AllChildListItems;

	for (UListDataObject_Base* ChildListData : FoundTabCollection->GetAllChildListData())
	{
		if (!ChildListData) continue;

		AllChildListItems.Add(ChildListData);
		if (ChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(ChildListData, AllChildListItems);
		}
	}
	
	return AllChildListItems;
}

void UOptionsDataRegistry::FindChildListDataRecursively(UListDataObject_Base* InParentData,
	TArray<UListDataObject_Base*>& OutFoundChildListData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData()) return;

	for (UListDataObject_Base* SubChildListData : InParentData->GetAllChildListData())
	{
		if (!SubChildListData) continue;
		
		OutFoundChildListData.Add(SubChildListData);

		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildListData, OutFoundChildListData);
		}
	}
}

void UOptionsDataRegistry::InitGameplayCollectionTab(ULocalPlayer* InOwningLocalPlayer)
{
	UListDataObject_Collection* GameplayTabCollection = NewObject<UListDataObject_Collection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("GameplayTitleKey"));
	
	//This is full code for constructor data interactor helper
	/*TSharedPtr<FOptionsDataInteractionHelper> ConstructedHelper = MakeShared<FOptionsDataInteractionHelper>(
		GET_FUNCTION_NAME_STRING_CHECKED(UVFGameUserSettings, GetCurrentGameDifficulty));*/

	FOptionsDataEditConditionDescriptor MainMenuOnlyCondition;
	if (InOwningLocalPlayer)
	{
		FString CurrentMapName = UGameplayStatics::GetCurrentLevelName(InOwningLocalPlayer, true);
		FString MainMenuMapName;
		TSoftObjectPtr<UWorld> SoftMainMenuWorld = UVFFunctionLibrary::GetVFGameInstance(InOwningLocalPlayer)->GetGameLevelByTag(VFGameplayTags::GameData_Level_MainMenuMap);

		if (!SoftMainMenuWorld.IsNull())
		{
			MainMenuMapName = SoftMainMenuWorld.GetAssetName();
		}
		MainMenuOnlyCondition.SetEditConditionFunc(
		[CurrentMapName, MainMenuMapName]()->bool
		{
			return CurrentMapName == MainMenuMapName;
		}
	);
	}
	
	MainMenuOnlyCondition.SetDisabledRichReason(UVFFunctionLibrary::GetOptionsScreenDescriptionText("MainMenuConditionKey").ToString());

	//Game Category
	{
		UListDataObject_Collection* GameCollection = NewObject<UListDataObject_Collection>();
		GameCollection->SetDataID(FName("GameCollection"));
		GameCollection->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("GameTitleKey"));

		GameplayTabCollection->AddChildListData(GameCollection);
		//Game Difficulty
		{
			UListDataObject_StringEnum* GameDifficulty = NewObject<UListDataObject_StringEnum>();
			GameDifficulty->SetDataID(FName("GameDifficulty"));
			GameDifficulty->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("DifficultTitleKey"));
			GameDifficulty->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("DifficultyDescKey"));
			GameDifficulty->AddEnumOption(EVFGameDifficulty::Easy, UVFFunctionLibrary::GetOptionsScreenTitleText("EasyKey"));
			GameDifficulty->AddEnumOption(EVFGameDifficulty::Normal, UVFFunctionLibrary::GetOptionsScreenTitleText("NormalKey"));
			GameDifficulty->AddEnumOption(EVFGameDifficulty::Hard, UVFFunctionLibrary::GetOptionsScreenTitleText("HardKey"));
			GameDifficulty->AddEnumOption(EVFGameDifficulty::VeryHard, UVFFunctionLibrary::GetOptionsScreenTitleText("VeryHardKey"));
			GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
			GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
			GameDifficulty->SetShouldApplySettingsImmediately(true);

			GameDifficulty->AddEditCondition(MainMenuOnlyCondition);

			GameplayTabCollection->AddChildListData(GameDifficulty);
		}
	}
	
	//Test Item
	/*{
		UListDataObject_String* TestItem = NewObject<UListDataObject_String>();
		TestItem->SetDataID(FName("TestItem"));
		TestItem->SetDataDisplayName(FText::FromString(TEXT("Test Item")));
		TestItem->SetSoftDescriptionImage(UVFFunctionLibrary::GetOptionsSoftImageByTag(VFGameplayTags::Frontend_Image_TestImage));
		TestItem->SetDescriptionRichText(FText::FromString(TEXT("The image to display can be specified in the project settings. It can be anything the developer assigned in there")));
		
		GameplayTabCollection->AddChildListData(TestItem);
	}*/

	//Camera Category
	{
		UListDataObject_Collection* CameraCollection = NewObject<UListDataObject_Collection>();
		CameraCollection->SetDataID(FName("CameraCollection"));
		CameraCollection->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("CameraTitleKey"));

		GameplayTabCollection->AddChildListData(CameraCollection);

		//Camera Zoom Distance
		{
			UListDataObject_Scalar* CameraZoom = NewObject<UListDataObject_Scalar>();
			CameraZoom->SetDataID(FName("CameraZoomDistance"));
			CameraZoom->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("CameraZoomDistanceKey"));
			CameraZoom->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("CameraZoomDescKey"));

			CameraZoom->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			CameraZoom->SetOutputValueRange(TRange<float>(0.f, 1.f));
			CameraZoom->SetSliderStepSize(0.01f);
			CameraZoom->SetDefaultValueFromString(LexToString(0.5f));
			CameraZoom->SetDisplayNumericType(ECommonNumericType::Percentage);
			CameraZoom->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());

			CameraZoom->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCameraZoomDistance));
			CameraZoom->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCameraZoomDistance));
			CameraZoom->SetShouldApplySettingsImmediately(true);

			GameplayTabCollection->AddChildListData(CameraZoom);
		}

		//Lock Vertical Camera
		{
			UListDataObject_StringBool* LockVerticalCamera = NewObject<UListDataObject_StringBool>();
			LockVerticalCamera->SetDataID(FName("LockVerticalCamera"));
			LockVerticalCamera->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("LockVerticalCameraKey"));
			LockVerticalCamera->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("LockVerticalCameraDescKey"));

			LockVerticalCamera->OverrideFalseDisplayText(UVFFunctionLibrary::GetOptionsScreenTitleText("OffKey"));
			LockVerticalCamera->OverrideTrueDisplayText(UVFFunctionLibrary::GetOptionsScreenTitleText("OnKey"));
			LockVerticalCamera->SetFalseAsDefaultValue();

			LockVerticalCamera->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetLockVerticalCamera));
			LockVerticalCamera->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetLockVerticalCamera));
			LockVerticalCamera->SetShouldApplySettingsImmediately(true);
			GameplayTabCollection->AddChildListData(LockVerticalCamera);
		}
	}

	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("AudioTitleKey"));

	//Volume Category
	{
		UListDataObject_Collection* VolumeCategoryCollection = NewObject<UListDataObject_Collection>();
		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("VolumeKey"));

		AudioTabCollection->AddChildListData(VolumeCategoryCollection);

		//Overall Volume
		{
			UListDataObject_Scalar* OverallVolume = NewObject<UListDataObject_Scalar>();
			OverallVolume->SetDataID(FName("OverallVolume"));
			OverallVolume->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("OverallVolumeKey"));
			OverallVolume->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenTitleText("SetOverallVolumeKey"));
			OverallVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			OverallVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			OverallVolume->SetSliderStepSize(0.01f);
			OverallVolume->SetDefaultValueFromString(LexToString(1.f));
			OverallVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			OverallVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			OverallVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallVolume));
			OverallVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallVolume));
			OverallVolume->SetShouldApplySettingsImmediately(true);

			VolumeCategoryCollection->AddChildListData(OverallVolume);
		}

		//Music Volume
		{
			UListDataObject_Scalar* MusicVolume = NewObject<UListDataObject_Scalar>();
			MusicVolume->SetDataID(FName("MusicVolume"));
			MusicVolume->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("MusicVolumeKey"));
			MusicVolume->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenTitleText("SetMusicVolumeKey"));
			MusicVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			MusicVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			MusicVolume->SetSliderStepSize(0.01f);
			MusicVolume->SetDefaultValueFromString(LexToString(1.f));
			MusicVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			MusicVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			MusicVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetMusicVolume));
			MusicVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMusicVolume));
			MusicVolume->SetShouldApplySettingsImmediately(true);

			VolumeCategoryCollection->AddChildListData(MusicVolume);
		}

		//Sound FX Volume
		{
			UListDataObject_Scalar* SoundFXVolume = NewObject<UListDataObject_Scalar>();
			SoundFXVolume->SetDataID(FName("SoundFXVolume"));
			SoundFXVolume->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("SoundFXVolumeKey"));
			SoundFXVolume->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenTitleText("SetSoundFXVolumeKey"));
			SoundFXVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			SoundFXVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			SoundFXVolume->SetSliderStepSize(0.01f);
			SoundFXVolume->SetDefaultValueFromString(LexToString(1.f));
			SoundFXVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			SoundFXVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			SoundFXVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetSoundFXVolume));
			SoundFXVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetSoundFXVolume));
			SoundFXVolume->SetShouldApplySettingsImmediately(true);

			VolumeCategoryCollection->AddChildListData(SoundFXVolume);
		}
		//UI Audio Volume
		{
			UListDataObject_Scalar* UIAudioVolume = NewObject<UListDataObject_Scalar>();
			UIAudioVolume->SetDataID(FName("UIAudioVolume"));
			UIAudioVolume->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("UIAudioVolumeKey"));
			UIAudioVolume->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenTitleText("SetUIAudioVolumeKey"));
			UIAudioVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			UIAudioVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			UIAudioVolume->SetSliderStepSize(0.01f);
			UIAudioVolume->SetDefaultValueFromString(LexToString(1.f));
			UIAudioVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			UIAudioVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			UIAudioVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetUIAudioVolume));
			UIAudioVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetUIAudioVolume));
			UIAudioVolume->SetShouldApplySettingsImmediately(true);

			VolumeCategoryCollection->AddChildListData(UIAudioVolume);
		}
		//Voice Volume
		{
			UListDataObject_Scalar* VoiceVolume = NewObject<UListDataObject_Scalar>();
			VoiceVolume->SetDataID(FName("VoiceVolume"));
			VoiceVolume->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("VoiceVolumeKey"));
			VoiceVolume->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenTitleText("SetVoiceVolumeKey"));
			VoiceVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			VoiceVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			VoiceVolume->SetSliderStepSize(0.01f);
			VoiceVolume->SetDefaultValueFromString(LexToString(1.f));
			VoiceVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			VoiceVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			VoiceVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetVoiceVolume));
			VoiceVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVoiceVolume));
			VoiceVolume->SetShouldApplySettingsImmediately(true);

			VolumeCategoryCollection->AddChildListData(VoiceVolume);
		}
		
	}

	/*//Sound Category
	{
		UListDataObject_Collection* SoundTabCollection = NewObject<UListDataObject_Collection>();
		SoundTabCollection->SetDataID(FName("SoundCategoryCollection"));
		SoundTabCollection->SetDataDisplayName(FText::FromString(TEXT("Sound")));

		AudioTabCollection->AddChildListData(SoundTabCollection);

		//Allow Background Audio
		{
			UListDataObject_StringBool* AllowBackgroundAudio = NewObject<UListDataObject_StringBool>();
			AllowBackgroundAudio->SetDataID(FName("AllowBackgroundAudio"));
			AllowBackgroundAudio->SetDataDisplayName(FText::FromString(TEXT("Use HDR Audio Mode")));
			AllowBackgroundAudio->OverrideTrueDisplayText(FText::FromString(TEXT("Enabled")));
			AllowBackgroundAudio->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
			AllowBackgroundAudio->SetFalseAsDefaultValue();
			AllowBackgroundAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowBackgroundAudio));
			AllowBackgroundAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowBackgroundAudio));
			AllowBackgroundAudio->SetShouldApplySettingsImmediately(true);

			SoundTabCollection->AddChildListData(AllowBackgroundAudio);
		}
		//Use HDR Audio
		{
			UListDataObject_StringBool* UseHDRAudioMode = NewObject<UListDataObject_StringBool>();
			UseHDRAudioMode->SetDataID(FName("UseHDRAudioMode"));
			UseHDRAudioMode->SetDataDisplayName(FText::FromString(TEXT("Use HDR Audio Mode")));
			UseHDRAudioMode->OverrideTrueDisplayText(FText::FromString(TEXT("Enabled")));
			UseHDRAudioMode->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
			UseHDRAudioMode->SetFalseAsDefaultValue();
			UseHDRAudioMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetUseHDRAudioMode));
			UseHDRAudioMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetUseHDRAudioMode));
			UseHDRAudioMode->SetShouldApplySettingsImmediately(true);

			SoundTabCollection->AddChildListData(UseHDRAudioMode);
		}
	}*/

	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("VideoTitleKey"));

	UListDataObject_StringEnum* CreatedWindowMode = nullptr;

	//Display Category
	{
		UListDataObject_Collection* DisplayCategoryCollection = NewObject<UListDataObject_Collection>();
		DisplayCategoryCollection->SetDataID(FName("DisplayCategoryCollection"));
		DisplayCategoryCollection->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("DisplayKey"));
		
		VideoTabCollection->AddChildListData(DisplayCategoryCollection);

		FOptionsDataEditConditionDescriptor PackagedBuildOnlyCondition;
		PackagedBuildOnlyCondition.SetEditConditionFunc(
				[]()->bool
				{
					const bool bIsInEditor = GIsEditor || GIsPlayInEditorWorld;
					
					return !bIsInEditor;
				}
			);
		PackagedBuildOnlyCondition.SetDisabledRichReason(UVFFunctionLibrary::GetOptionsScreenDescriptionText("PackagedBuildOnlyConditionKey").ToString());

		//Window Mode
		{
			/*const FText WindowModeDesc = LOCTABLE("/Game/UI/StringTable/ST_OptionsScreenDescription.ST_OptionsScreenDescription", "WindowModeDescKey");*/
			
			UListDataObject_StringEnum* WindowMode = NewObject<UListDataObject_StringEnum>();
			WindowMode->SetDataID(FName("WindowMode"));
			WindowMode->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("WindowModeKey"));
			WindowMode->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("WindowModeDescKey"));
			WindowMode->AddEnumOption(EWindowMode::Fullscreen, UVFFunctionLibrary::GetOptionsScreenTitleText("FullscreenKey"));
			WindowMode->AddEnumOption(EWindowMode::WindowedFullscreen, UVFFunctionLibrary::GetOptionsScreenTitleText("WindowedFullscreenKey"));
			WindowMode->AddEnumOption(EWindowMode::Windowed, UVFFunctionLibrary::GetOptionsScreenTitleText("WindowedKey"));
			WindowMode->SetDefaultFromEnumOption(EWindowMode::WindowedFullscreen);
			WindowMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			WindowMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));
			WindowMode->SetShouldApplySettingsImmediately(true);

			WindowMode->AddEditCondition(PackagedBuildOnlyCondition);

			CreatedWindowMode = WindowMode;

			DisplayCategoryCollection->AddChildListData(WindowMode);
		}
		//Screen Resolution
		{
			UListDataObject_StringResolution* ScreenResolution = NewObject<UListDataObject_StringResolution>();
			ScreenResolution->SetDataID(FName("ScreenResolution"));
			ScreenResolution->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("ScreenResolutionKey"));
			ScreenResolution->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("ScreenResolutionsDescKey"));
			ScreenResolution->InitResolutionValues();
			ScreenResolution->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetScreenResolution));
			ScreenResolution->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetScreenResolution));
			ScreenResolution->SetShouldApplySettingsImmediately(true);

			ScreenResolution->AddEditCondition(PackagedBuildOnlyCondition);

			FOptionsDataEditConditionDescriptor WindowModeEditorCondition;
			WindowModeEditorCondition.SetEditConditionFunc(
				[CreatedWindowMode]()->bool
					{
						const bool bIsBorderlessMode = CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::WindowedFullscreen;

						return !bIsBorderlessMode;
					}
				);
			WindowModeEditorCondition.SetDisabledRichReason(UVFFunctionLibrary::GetOptionsScreenDescriptionText("WindowModeEditorConditionKey").ToString());
			WindowModeEditorCondition.SetDisabledForcedStringValue(ScreenResolution->GetMaximumAllowedResolution());

			ScreenResolution->AddEditCondition(WindowModeEditorCondition);

			ScreenResolution->AddEditDependencyData(CreatedWindowMode);

			DisplayCategoryCollection->AddChildListData(ScreenResolution);
		}
	}

	//Graphics Category
	{
		UListDataObject_Collection* GraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
		GraphicsCategoryCollection->SetDataID(FName("GraphicsCategoryCollection"));
		GraphicsCategoryCollection->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("GraphicsKey"));

		VideoTabCollection->AddChildListData(GraphicsCategoryCollection);

		//Display Gamma
		{
			UListDataObject_Scalar* DisplayGamma = NewObject<UListDataObject_Scalar>();
			DisplayGamma->SetDataID(FName("DisplayGamma"));
			DisplayGamma->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("BrightnessKey"));
			DisplayGamma->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("DisplayGammaDescKey"));
			DisplayGamma->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			DisplayGamma->SetOutputValueRange(TRange<float>(1.7f, 2.7f));//The default value Unreal has is 2.2f
			DisplayGamma->SetSliderStepSize(0.01f);
			DisplayGamma->SetDisplayNumericType(ECommonNumericType::Percentage);
			DisplayGamma->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			DisplayGamma->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentDisplayGamma));
			DisplayGamma->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentDisplayGamma));
			DisplayGamma->SetDefaultValueFromString(LexToString(2.2f));

			GraphicsCategoryCollection->AddChildListData(DisplayGamma);
		}
		UListDataObject_StringInteger* CreatedOverallQuality = nullptr;
		
		//Overall Quality
		{
			UListDataObject_StringInteger* OverallQuality = NewObject<UListDataObject_StringInteger>();
			OverallQuality->SetDataID(FName("OverallQuality"));
			OverallQuality->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("OverallQualityKey"));
			OverallQuality->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("OverallQualityDescKey"));
			OverallQuality->AddIntegerOption(0, UVFFunctionLibrary::GetOptionsScreenTitleText("LowKey"));
			OverallQuality->AddIntegerOption(1, UVFFunctionLibrary::GetOptionsScreenTitleText("MediumKey"));
			OverallQuality->AddIntegerOption(2, UVFFunctionLibrary::GetOptionsScreenTitleText("HighKey"));
			OverallQuality->AddIntegerOption(3, UVFFunctionLibrary::GetOptionsScreenTitleText("EpicKey"));
			OverallQuality->AddIntegerOption(4, UVFFunctionLibrary::GetOptionsScreenTitleText("CinematicKey"));
			OverallQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallScalabilityLevel));
			OverallQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallScalabilityLevel));
			OverallQuality->SetShouldApplySettingsImmediately(true);

			GraphicsCategoryCollection->AddChildListData(OverallQuality);

			CreatedOverallQuality = OverallQuality;
		}

		//Resolution Scale
		{
			UListDataObject_Scalar* ResolutionScale = NewObject<UListDataObject_Scalar>();
			ResolutionScale->SetDataID(FName("ResolutionScale"));
			ResolutionScale->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("ResolutionScaleKey"));
			ResolutionScale->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("ResolutionScaleDescKey"));
			ResolutionScale->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetOutputValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetSliderStepSize(0.01f);
			ResolutionScale->SetDisplayNumericType(ECommonNumericType::Percentage);
			ResolutionScale->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			ResolutionScale->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetResolutionScaleNormalized));
			ResolutionScale->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetResolutionScaleNormalized));
			ResolutionScale->SetShouldApplySettingsImmediately(true);

			ResolutionScale->AddEditDependencyData(CreatedOverallQuality);

			GraphicsCategoryCollection->AddChildListData(ResolutionScale);
		}

		//Global Illumination Quality
		{
			UListDataObject_StringInteger* GlobalIlluminationQuality = NewObject<UListDataObject_StringInteger>();
			GlobalIlluminationQuality->SetDataID(FName("GlobalIlluminationQuality"));
			GlobalIlluminationQuality->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("GlobalIlluminationQualityKey"));
			GlobalIlluminationQuality->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("GlobalIlluminationQualityDescKey"));
			GlobalIlluminationQuality->AddIntegerOption(0, UVFFunctionLibrary::GetOptionsScreenTitleText("LowKey"));
			GlobalIlluminationQuality->AddIntegerOption(1, UVFFunctionLibrary::GetOptionsScreenTitleText("MediumKey"));
			GlobalIlluminationQuality->AddIntegerOption(2, UVFFunctionLibrary::GetOptionsScreenTitleText("HighKey"));
			GlobalIlluminationQuality->AddIntegerOption(3, UVFFunctionLibrary::GetOptionsScreenTitleText("EpicKey"));
			GlobalIlluminationQuality->AddIntegerOption(4, UVFFunctionLibrary::GetOptionsScreenTitleText("CinematicKey"));
			GlobalIlluminationQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetShouldApplySettingsImmediately(true);

			GlobalIlluminationQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(GlobalIlluminationQuality);

			GraphicsCategoryCollection->AddChildListData(GlobalIlluminationQuality);
		}

		//Shadow Quality
		{
			UListDataObject_StringInteger* ShadowQuality = NewObject<UListDataObject_StringInteger>();
			ShadowQuality->SetDataID(FName("ShadowQuality"));
			ShadowQuality->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("ShadowQualityKey"));
			ShadowQuality->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("ShadowQualityDescKey"));
			ShadowQuality->AddIntegerOption(0, UVFFunctionLibrary::GetOptionsScreenTitleText("LowKey"));
			ShadowQuality->AddIntegerOption(1, UVFFunctionLibrary::GetOptionsScreenTitleText("MediumKey"));
			ShadowQuality->AddIntegerOption(2, UVFFunctionLibrary::GetOptionsScreenTitleText("HighKey"));
			ShadowQuality->AddIntegerOption(3, UVFFunctionLibrary::GetOptionsScreenTitleText("EpicKey"));
			ShadowQuality->AddIntegerOption(4, UVFFunctionLibrary::GetOptionsScreenTitleText("CinematicKey"));
			ShadowQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetShadowQuality));
			ShadowQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetShadowQuality));
			ShadowQuality->SetShouldApplySettingsImmediately(true);

			ShadowQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ShadowQuality);

			GraphicsCategoryCollection->AddChildListData(ShadowQuality);
		}

		//AntiAliasing Quality
		{
			UListDataObject_StringInteger* AntiAliasingQuality = NewObject<UListDataObject_StringInteger>();
			AntiAliasingQuality->SetDataID(FName("AntiAliasingQuality"));
			AntiAliasingQuality->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("AntiAliasingQualityKey"));
			AntiAliasingQuality->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("AntiAliasingDescKey"));
			AntiAliasingQuality->AddIntegerOption(0, UVFFunctionLibrary::GetOptionsScreenTitleText("LowKey"));
			AntiAliasingQuality->AddIntegerOption(1, UVFFunctionLibrary::GetOptionsScreenTitleText("MediumKey"));
			AntiAliasingQuality->AddIntegerOption(2, UVFFunctionLibrary::GetOptionsScreenTitleText("HighKey"));
			AntiAliasingQuality->AddIntegerOption(3, UVFFunctionLibrary::GetOptionsScreenTitleText("EpicKey"));
			AntiAliasingQuality->AddIntegerOption(4, UVFFunctionLibrary::GetOptionsScreenTitleText("CinematicKey"));
			AntiAliasingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAntiAliasingQuality));
			AntiAliasingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAntiAliasingQuality));
			AntiAliasingQuality->SetShouldApplySettingsImmediately(true);

			AntiAliasingQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(AntiAliasingQuality);

			GraphicsCategoryCollection->AddChildListData(AntiAliasingQuality);
		}

		//View Distance Quality
		{
			UListDataObject_StringInteger* ViewDistanceQuality = NewObject<UListDataObject_StringInteger>();
			ViewDistanceQuality->SetDataID(FName("ViewDistanceQuality"));
			ViewDistanceQuality->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("ViewDistanceQualityKey"));
			ViewDistanceQuality->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("ViewDistanceDescKey"));
			ViewDistanceQuality->AddIntegerOption(0, UVFFunctionLibrary::GetOptionsScreenTitleText("LowKey"));
			ViewDistanceQuality->AddIntegerOption(1, UVFFunctionLibrary::GetOptionsScreenTitleText("MediumKey"));
			ViewDistanceQuality->AddIntegerOption(2, UVFFunctionLibrary::GetOptionsScreenTitleText("HighKey"));
			ViewDistanceQuality->AddIntegerOption(3, UVFFunctionLibrary::GetOptionsScreenTitleText("EpicKey"));
			ViewDistanceQuality->AddIntegerOption(4, UVFFunctionLibrary::GetOptionsScreenTitleText("CinematicKey"));
			ViewDistanceQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetViewDistanceQuality));
			ViewDistanceQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetViewDistanceQuality));
			ViewDistanceQuality->SetShouldApplySettingsImmediately(true);

			ViewDistanceQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ViewDistanceQuality);

			GraphicsCategoryCollection->AddChildListData(ViewDistanceQuality);
		}

		//Texture Quality
		{
			UListDataObject_StringInteger* TextureQuality = NewObject<UListDataObject_StringInteger>();
			TextureQuality->SetDataID(FName("TextureQuality"));
			TextureQuality->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("TextureQualityKey"));
			TextureQuality->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("TextureQualityDescKey"));
			TextureQuality->AddIntegerOption(0, UVFFunctionLibrary::GetOptionsScreenTitleText("LowKey"));
			TextureQuality->AddIntegerOption(1, UVFFunctionLibrary::GetOptionsScreenTitleText("MediumKey"));
			TextureQuality->AddIntegerOption(2, UVFFunctionLibrary::GetOptionsScreenTitleText("HighKey"));
			TextureQuality->AddIntegerOption(3, UVFFunctionLibrary::GetOptionsScreenTitleText("EpicKey"));
			TextureQuality->AddIntegerOption(4, UVFFunctionLibrary::GetOptionsScreenTitleText("CinematicKey"));
			TextureQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetTextureQuality));
			TextureQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetTextureQuality));
			TextureQuality->SetShouldApplySettingsImmediately(true);

			TextureQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(TextureQuality);

			GraphicsCategoryCollection->AddChildListData(TextureQuality);
		}

		//Visual Effects Quality
		{
			UListDataObject_StringInteger* VisualEffectQuality = NewObject<UListDataObject_StringInteger>();
			VisualEffectQuality->SetDataID(FName("VisualEffectQuality"));
			VisualEffectQuality->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("VisualEffectQualityKey"));
			VisualEffectQuality->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("VisualEffectQualityDescKey"));
			VisualEffectQuality->AddIntegerOption(0, UVFFunctionLibrary::GetOptionsScreenTitleText("LowKey"));
			VisualEffectQuality->AddIntegerOption(1, UVFFunctionLibrary::GetOptionsScreenTitleText("MediumKey"));
			VisualEffectQuality->AddIntegerOption(2, UVFFunctionLibrary::GetOptionsScreenTitleText("HighKey"));
			VisualEffectQuality->AddIntegerOption(3, UVFFunctionLibrary::GetOptionsScreenTitleText("EpicKey"));
			VisualEffectQuality->AddIntegerOption(4, UVFFunctionLibrary::GetOptionsScreenTitleText("CinematicKey"));
			VisualEffectQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetVisualEffectQuality));
			VisualEffectQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVisualEffectQuality));
			VisualEffectQuality->SetShouldApplySettingsImmediately(true);

			VisualEffectQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(VisualEffectQuality);

			GraphicsCategoryCollection->AddChildListData(VisualEffectQuality);
		}

		//Reflection Quality
		{
			UListDataObject_StringInteger* ReflectionQuality = NewObject<UListDataObject_StringInteger>();
			ReflectionQuality->SetDataID(FName("ReflectionQuality"));
			ReflectionQuality->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("ReflectionQualityKey"));
			ReflectionQuality->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("ReflectionQualityDescKey"));
			ReflectionQuality->AddIntegerOption(0, UVFFunctionLibrary::GetOptionsScreenTitleText("LowKey"));
			ReflectionQuality->AddIntegerOption(1, UVFFunctionLibrary::GetOptionsScreenTitleText("MediumKey"));
			ReflectionQuality->AddIntegerOption(2, UVFFunctionLibrary::GetOptionsScreenTitleText("HighKey"));
			ReflectionQuality->AddIntegerOption(3, UVFFunctionLibrary::GetOptionsScreenTitleText("EpicKey"));
			ReflectionQuality->AddIntegerOption(4, UVFFunctionLibrary::GetOptionsScreenTitleText("CinematicKey"));
			ReflectionQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetReflectionQuality));
			ReflectionQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetReflectionQuality));
			ReflectionQuality->SetShouldApplySettingsImmediately(true);

			ReflectionQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ReflectionQuality);

			GraphicsCategoryCollection->AddChildListData(ReflectionQuality);
		}

		//Post Processing Quality
		{
			UListDataObject_StringInteger* PostProcessingQuality = NewObject<UListDataObject_StringInteger>();
			PostProcessingQuality->SetDataID(FName("PostProcessingQuality"));
			PostProcessingQuality->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("PostProcessingQualityKey"));
			PostProcessingQuality->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("PostProcessingQualityDescKey"));
			PostProcessingQuality->AddIntegerOption(0, UVFFunctionLibrary::GetOptionsScreenTitleText("LowKey"));
			PostProcessingQuality->AddIntegerOption(1, UVFFunctionLibrary::GetOptionsScreenTitleText("MediumKey"));
			PostProcessingQuality->AddIntegerOption(2, UVFFunctionLibrary::GetOptionsScreenTitleText("HighKey"));
			PostProcessingQuality->AddIntegerOption(3, UVFFunctionLibrary::GetOptionsScreenTitleText("EpicKey"));
			PostProcessingQuality->AddIntegerOption(4, UVFFunctionLibrary::GetOptionsScreenTitleText("CinematicKey"));
			PostProcessingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPostProcessingQuality));
			PostProcessingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPostProcessingQuality));
			PostProcessingQuality->SetShouldApplySettingsImmediately(true);

			PostProcessingQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(PostProcessingQuality);

			GraphicsCategoryCollection->AddChildListData(PostProcessingQuality);
		}
	}

	//Advanced Graphics Category
	{
		UListDataObject_Collection* AdvancedGraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
		AdvancedGraphicsCategoryCollection->SetDataID(FName("AdvancedGraphicsCategoryCollection"));
		AdvancedGraphicsCategoryCollection->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("AdvancedGraphicsKey"));

		VideoTabCollection->AddChildListData(AdvancedGraphicsCategoryCollection);

		//Vertical Sync
		{
			UListDataObject_StringBool* VerticalSync = NewObject<UListDataObject_StringBool>();
			VerticalSync->SetDataID(FName("VerticalSync"));
			VerticalSync->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("VerticalSyncKey"));
			VerticalSync->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("VerticalSyncDescKey"));
			VerticalSync->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsVSyncEnabled));
			VerticalSync->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVSyncEnabled));
			VerticalSync->SetFalseAsDefaultValue();
			VerticalSync->SetShouldApplySettingsImmediately(true);

			FOptionsDataEditConditionDescriptor FullScreenOnlyCondition;
			FullScreenOnlyCondition.SetEditConditionFunc(
				[CreatedWindowMode]()->bool
				{
					return CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Fullscreen;
				}
			);
			FullScreenOnlyCondition.SetDisabledRichReason(UVFFunctionLibrary::GetOptionsScreenDescriptionText("FullScreenOnlyConditionKey").ToString());
			FullScreenOnlyCondition.SetDisabledForcedStringValue(TEXT("false"));

			VerticalSync->AddEditCondition(FullScreenOnlyCondition);

			AdvancedGraphicsCategoryCollection->AddChildListData(VerticalSync);
		}

		//Frame Rate Limit
		{
			UListDataObject_String* FrameRateLimit = NewObject<UListDataObject_String>();
			FrameRateLimit->SetDataID(FName("FrameRateLimit"));
			FrameRateLimit->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("FrameRateLimitKey"));
			FrameRateLimit->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("FrameRateLimitDescKey"));
			FrameRateLimit->AddDynamicOption(LexToString(30.f), FText::FromString(TEXT("30 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(60.f), FText::FromString(TEXT("60 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(90.f), FText::FromString(TEXT("90 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(120.f), FText::FromString(TEXT("120 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(0.f), UVFFunctionLibrary::GetOptionsScreenTitleText("NoLimitKey"));
			FrameRateLimit->SetDefaultValueFromString(LexToString(0.f));
			FrameRateLimit->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFrameRateLimit));
			FrameRateLimit->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFrameRateLimit));
			FrameRateLimit->SetShouldApplySettingsImmediately(true);

			AdvancedGraphicsCategoryCollection->AddChildListData(FrameRateLimit);
		}
	}
	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

void UOptionsDataRegistry::InitControlCollectionTab(ULocalPlayer* InOwningLocalPlayer)
{
	UListDataObject_Collection* ControlTabCollection = NewObject<UListDataObject_Collection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("ControlKey"));

	UEnhancedInputLocalPlayerSubsystem* EISubsystem = InOwningLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	check(EISubsystem);

	UEnhancedInputUserSettings* EIUserSettings = EISubsystem->GetUserSettings();

	check(EIUserSettings);
	
	//Keyboard Mouse Category
	{
		UListDataObject_Collection* KeyBoardMouseCategoryCollection = NewObject<UListDataObject_Collection>();
		KeyBoardMouseCategoryCollection->SetDataID(FName("KeyBoardMouseCategoryCollection"));
		KeyBoardMouseCategoryCollection->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("KeyBoardMouseKey"));

		ControlTabCollection->AddChildListData(KeyBoardMouseCategoryCollection);

		//Keyboard Mouse Input
		{
			FPlayerMappableKeyQueryOptions KeyboardMouseOnly;
			KeyboardMouseOnly.KeyToMatch = EKeys::A;
			KeyboardMouseOnly.bMatchBasicKeyTypes = true;
			
			for (const TTuple<FString, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>& ProfilePair : EIUserSettings->GetAllAvailableKeyProfiles())
			{
				UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;

				check(MappableKeyProfile);

				for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
				{
					for (const FPlayerKeyMapping& KeyMapping : MappingRowPair.Value.Mappings)
					{
						if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMapping, KeyboardMouseOnly))
						{
							/*Debug::Print(
							TEXT("Mapping ID:") + KeyMapping.GetMappingName().ToString() +
							TEXT(" Display Name:") + KeyMapping.GetDisplayName().ToString() +
							TEXT(" Bound Key:") + KeyMapping.GetCurrentKey().GetDisplayName().ToString() 
							);*/

							UListDataObject_KeyRemap* KeyRemapDataObject = NewObject<UListDataObject_KeyRemap>();
							KeyRemapDataObject->SetDataID(KeyMapping.GetMappingName());
							KeyRemapDataObject->SetDataDisplayName(KeyMapping.GetDisplayName());
							KeyRemapDataObject->InitKeyRemapData(EIUserSettings, MappableKeyProfile, ECommonInputType::MouseAndKeyboard, KeyMapping);
							KeyRemapDataObject->SetShouldApplySettingsImmediately(true);

							FString DynamicDescKey = KeyMapping.GetMappingName().ToString() + TEXT("_DescKey");

							FText DescriptionText = UVFFunctionLibrary::GetOptionsScreenDescriptionText(FName(DynamicDescKey));

							if (!DescriptionText.IsEmpty())
							{
								KeyRemapDataObject->SetDescriptionRichText(DescriptionText);
							}

							if (KeyRemapDataObject->GetDataDisplayName().ToString() == TEXT_EMPTY) continue;

							KeyBoardMouseCategoryCollection->AddChildListData(KeyRemapDataObject);
						}
					}
				}
			}
		}
	}

	//Gamepad Category
	{
		UListDataObject_Collection* GamepadCategoryCollection = NewObject<UListDataObject_Collection>();
		GamepadCategoryCollection->SetDataID(FName("GamepadCategoryCollection"));
		GamepadCategoryCollection->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("GamepadKey"));

		ControlTabCollection->AddChildListData(GamepadCategoryCollection);
		//Gamepad Input
		{
			FPlayerMappableKeyQueryOptions GamepadOnly;
			GamepadOnly.KeyToMatch = EKeys::Gamepad_FaceButton_Bottom;
			GamepadOnly.bMatchBasicKeyTypes = true;

			for (const TTuple<FString, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>& ProfilePair : EIUserSettings->GetAllAvailableKeyProfiles())
			{
				UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;

				check(MappableKeyProfile);

				for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
				{
					for (const FPlayerKeyMapping& KeyMapping : MappingRowPair.Value.Mappings)
					{
						if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMapping, GamepadOnly))
						{
							/*Debug::Print(
							TEXT("Mapping ID:") + KeyMapping.GetMappingName().ToString() +
							TEXT(" Display Name:") + KeyMapping.GetDisplayName().ToString() +
							TEXT(" Bound Key:") + KeyMapping.GetCurrentKey().GetDisplayName().ToString() 
							);*/

							UListDataObject_KeyRemap* KeyRemapDataObject = NewObject<UListDataObject_KeyRemap>();
							KeyRemapDataObject->SetDataID(KeyMapping.GetMappingName());
							KeyRemapDataObject->SetDataDisplayName(KeyMapping.GetDisplayName());
							KeyRemapDataObject->InitKeyRemapData(EIUserSettings, MappableKeyProfile, ECommonInputType::Gamepad, KeyMapping);
							KeyRemapDataObject->SetShouldApplySettingsImmediately(true);

							FString DynamicDescKey = KeyMapping.GetMappingName().ToString() + TEXT("_DescKey");

							FText DescriptionText = UVFFunctionLibrary::GetOptionsScreenDescriptionText(FName(DynamicDescKey));

							if (!DescriptionText.IsEmpty())
							{
								KeyRemapDataObject->SetDescriptionRichText(DescriptionText);
							}

							if (KeyRemapDataObject->GetDataDisplayName().ToString() == TEXT_EMPTY) continue;

							GamepadCategoryCollection->AddChildListData(KeyRemapDataObject);
						}
					}
				}
			}
		}
	}
	
	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}

void UOptionsDataRegistry::InitLanguageCollectionTab()
{
	UListDataObject_Collection* LanguageTabCollection = NewObject<UListDataObject_Collection>();
	LanguageTabCollection->SetDataID(FName("LanguageTabCollection"));
	LanguageTabCollection->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("LanguageKey"));

	UListDataObject_Collection* TextCategoryCollection = NewObject<UListDataObject_Collection>();
	TextCategoryCollection->SetDataID(FName("TextCategoryCollection"));
	TextCategoryCollection->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("TextKey"));

	LanguageTabCollection->AddChildListData(TextCategoryCollection);

	{
		UListDataObject_String* GameLanguage = NewObject<UListDataObject_String>();
		GameLanguage->SetDataID(FName("GameLanguage"));
		GameLanguage->SetDataDisplayName(UVFFunctionLibrary::GetOptionsScreenTitleText("GameLanguageKey"));

		GameLanguage->SetDescriptionRichText(UVFFunctionLibrary::GetOptionsScreenDescriptionText("GameLanguageDescKey"));

		GameLanguage->AddDynamicOption(TEXT("en"), FText::FromString(TEXT("English")));
		GameLanguage->AddDynamicOption(TEXT("zh-Hant-TW"), FText::FromString(TEXT("繁體中文")));
		GameLanguage->AddDynamicOption(TEXT("ja-JP"), FText::FromString(TEXT("日本語")));
		GameLanguage->AddDynamicOption(TEXT("zh-Hans-CN"), FText::FromString(TEXT("简体中文")));

		GameLanguage->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentLanguage));
		GameLanguage->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentLanguage));

		GameLanguage->SetShouldApplySettingsImmediately(true);

		TextCategoryCollection->AddChildListData(GameLanguage);
	}

	RegisteredOptionsTabCollections.Add(LanguageTabCollection);

}
