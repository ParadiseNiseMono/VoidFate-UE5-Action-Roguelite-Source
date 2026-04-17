// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameModes/VFBaseGameMode.h"
#include "VFTutorialGameMode.generated.h"

// 定義教學的各個階段
UENUM(BlueprintType)
enum class EVFTutorialPhase : uint8
{
	None			UMETA(DisplayName = "Not Started"),
	Movement		UMETA(DisplayName = "Movement & Sprint"),
	DummyCombat		UMETA(DisplayName = "Basic Combat"),
	PerfectDodge	UMETA(DisplayName = "Perfect Dodge & Void Time"),
	ParryPractice	UMETA(DisplayName = "Parry & Execute"),
	Reward			UMETA(DisplayName = "Reward & Player State"),
	Finish			UMETA(DisplayName = "Finish"),
	Max				UMETA(Hidden)
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTutorialPhaseChangedDelegate, EVFTutorialPhase, OldPhase, EVFTutorialPhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerfectDodgeCountChangedDelegate, int32, NewCount);

/**
 * 教學關卡專用的 GameMode
 * 負責管控教學流程狀態，並透過 Delegate 廣播給 UI 和關卡藍圖，達成完全解耦。
 */
UCLASS()
class VOIDFATE_API AVFTutorialGameMode : public AVFBaseGameMode
{
	GENERATED_BODY()
	
public:
	AVFTutorialGameMode();

	UFUNCTION(BlueprintPure, Category = "Tutorial|State")
	EVFTutorialPhase GetCurrentTutorialPhase() const { return CurrentPhase; }

	UFUNCTION(BlueprintCallable, Category = "Tutorial|State")
	void SetTutorialPhase(EVFTutorialPhase NewPhase);

	UFUNCTION(BlueprintCallable, Category = "Tutorial|State")
	void AdvanceToNextPhase();

	UFUNCTION(BlueprintCallable, Category = "Tutorial|PerfectDodge")
	void IncrementPerfectDodgeCount();

	UFUNCTION(BlueprintPure, Category = "Tutorial|PerfectDodge")
	int32 GetPerfectDodgeCount() const { return PerfectDodgeTimes; }

	UPROPERTY(BlueprintAssignable, Category = "Tutorial|Events")
	FOnTutorialPhaseChangedDelegate OnTutorialPhaseChanged;

	UPROPERTY(BlueprintAssignable, Category = "Tutorial|Events")
	FOnPerfectDodgeCountChangedDelegate OnPerfectDodgeCountChanged;

protected:
	virtual void BeginPlay() override;

private:
	// 儲存當前階段
	UPROPERTY(VisibleAnywhere, Category = "Tutorial|State")
	EVFTutorialPhase CurrentPhase;

	// 完美閃避的次數，教學關卡會根據這個數值來決定何時進入下一個階段
	UPROPERTY(VisibleAnywhere, Category = "Tutorial|PerfectDodge")
	int32 PerfectDodgeTimes = 0;
};
