// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VFBGMSubsystem.generated.h"

class UAudioComponent;
class USoundBase;

/**
 * 專門負責管理全局 BGM 播放與淡入淡出的子系統
 */
UCLASS()
class VOIDFATE_API UVFBGMSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// 播放新的 BGM，並自動處理與上一首 BGM 的淡入淡出交替
	UFUNCTION(BlueprintCallable, Category = "VF|BGM")
	void PlayBGM(USoundBase* NewBGM, float FadeInTime = 1.5f, float FadeOutTime = 1.5f);

	// 停止當前所有的 BGM
	UFUNCTION(BlueprintCallable, Category = "VF|BGM")
	void StopBGM(float FadeOutTime = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "VF|BGM")
	void PlayGameModeBGM();

	UFUNCTION(BlueprintCallable, Category = "VF|BGM")
	void PlayMainMenuBGM();

protected:
	//~ Begin UGameInstanceSubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//~ End UGameInstanceSubsystem interface

private:
	// 正在播放的 BGM 音軌
	UPROPERTY()
	TObjectPtr<UAudioComponent> CurrentBGMComponent;

	// 正在淡出的舊 BGM 音軌 (保留指標以防被 Garbage Collection 提早回收)
	UPROPERTY()
	TObjectPtr<UAudioComponent> FadingBGMComponent;
};
