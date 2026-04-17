// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VFTypes/VFEnumTypes.h"
#include "VFBaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class VOIDFATE_API AVFBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVFBaseGameMode();

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	/*
	 * 設定遊戲的難度，但目前還沒想好要用甚麼方式設定，之後有需要再弄
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
	EVFGameDifficulty CurrentGameDifficulty;

public:
	UFUNCTION(BlueprintCallable, Category = "Game Settings")
	EVFGameDifficulty GetCurrentGameDifficulty() const { return CurrentGameDifficulty;}
};
