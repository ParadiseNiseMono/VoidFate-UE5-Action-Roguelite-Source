// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VFTypes/VFEnumTypes.h"
#include "CommonInputTypeEnum.h"
#include "VFFunctionLibrary.generated.h"

class UVFGameInstance;
struct FGameplayEffectSpecHandle;
struct FGameplayEventData;
struct FScalableFloat;
class UPawnCombatComponent;
class UVFAbilitySystemComponent;
class UAbilitySystemComponent;


class UVFGameUserSettings;
class UWidget_ActivatableBase;
class UInputAction;
/**
 * 
 */
UCLASS()
class VOIDFATE_API UVFFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Frontend Function Library")
	static TSoftClassPtr<UWidget_ActivatableBase> GetFrontendSoftWidgetClassByTag( UPARAM(meta = (Categories = "Frontend.Widget")) FGameplayTag InWidgetTag);

	UFUNCTION(BlueprintPure, Category = "Frontend Function Library")
	static TSoftObjectPtr<UTexture2D> GetOptionsSoftImageByTag(UPARAM(meta = (Categories = "Frontend.Image")) FGameplayTag InImageTag);

	UFUNCTION(BlueprintPure, Category = "Frontend Function Library")
	static UVFGameUserSettings* GetVFGameUserSettings();

	static UVFAbilitySystemComponent* NativeGetVFASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "VoidFate|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "VoidFate|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "VoidFate|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor,FGameplayTag TagToCheck,EVFConfirmType& OutConfirmType);

	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "VoidFate|FunctionLibrary", meta = (DisplayName = "Get Pawn Combat Component From Actor", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EVFValidType& OutValidType);

	UFUNCTION(BlueprintPure, Category = "VoidFate|FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn,APawn* TargetPawn);

	UFUNCTION(BlueprintPure, Category = "VoidFate|FunctionLibrary", meta = (CompactNodeTitle = "Get Value At Level"))
	static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat,float InLevel = 1.f);
	
	UFUNCTION(BlueprintPure, Category = "VoidFate|FunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(AActor* InAttacker,AActor* InVictim,float& OutAngleDifference);

	UFUNCTION(BlueprintPure, Category = "VoidFate|FunctionLibrary")
	static bool IsValidBlock(AActor* InAttacker,AActor* InDefender);

	/**
	 * 尋找範圍內最佳的敵人目標
	 * @param SourceActor       發起搜尋的人 (通常是玩家)
	 * @param SearchRadius      搜尋半徑
	 * @param SearchAngle       搜尋角度 (180 = 全方位, 90 = 前方半圓, 45 = 前方錐形)
	 * @param bDebugDraw        是否畫出除錯線
	 */
	UFUNCTION(BlueprintCallable, Category = "VoidFate|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static AActor* FindNearestTargetInRadius(
		const UObject* WorldContextObject,
		AActor* SourceActor,
		float SearchRadius = 500.0f,
		float SearchAngle = 180.0f,
		bool bDebugDraw = false
	);

	/**
	 * 計算攻擊時的最佳站位 (Warp Location) 與 面向 (Warp Rotation)
	 * @param Attacker          攻擊者
	 * @param Target            目標敵人
	 * @param AttackRange       攻擊距離 (例如刀長 150)
	 * @param OutLocation       (輸出) 計算出的站位
	 * @param OutRotation       (輸出) 計算出的面向
	 * @return                  是否計算成功
	 */
	UFUNCTION(BlueprintCallable, Category = "VF|Combat")
	static bool GetCombatWarpLocation(
		AActor* Attacker, 
		AActor* Target, 
		float AttackRange, 
		FVector& OutLocation, 
		FRotator& OutRotation
	);

	UFUNCTION(BlueprintPure, Category = "VoidFate|FunctionLibrary")
	static FGameplayEventData MakeGameplayEventDataWithHitResult(AActor* InInstigator, AActor* InTargetActor, const FHitResult& InHitResult);

	UFUNCTION(BlueprintCallable,Category = "VoidFate|FunctionLibrary")
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator,AActor* InTargetActor,const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "VoidFate|FunctionLibrary", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", ExpandEnumAsExecs = "CountDownInput|CountDownOutput",TotalTime = "1.0",UpdateInterval = "0.1"))
	static void CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval,
	float& OutRemainingTime, EVFCountDownActionInput CountDownInput,
	UPARAM(DisplayName = "Output") EVFCountDownActionOutput & CountDownOutput, FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintPure, Category = "VoidFate|UI")
	static FText GetOptionsScreenDescriptionText(FName Key);

	UFUNCTION(BlueprintPure, Category = "VoidFate|UI")
	static FText GetOptionsScreenTitleText(FName Key);

	UFUNCTION(BlueprintPure, Category = "VoidFate|UI")
	static FText GetModalScreenTitleText(FName Key);

	UFUNCTION(BlueprintPure, Category = "VoidFate|UI")
	static FText GetPlayerStateScreenText(FName Key);

	UFUNCTION(BlueprintPure, Category = "VoidFate|UI")
	static FText GetLoadingScreenText(FName Key);

	UFUNCTION(BlueprintPure, Category = "VoidFate|FunctionLibrary")
	static USoundBase* GetBGMByGameDifficulty(EVFGameDifficulty InDifficulty);

	UFUNCTION(BlueprintPure, Category = "VoidFate|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static UVFGameInstance* GetVFGameInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "VoidFate|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static void ToggleInputMode(const UObject* WorldContextObject, EVFInputMode InInputMode);

	/**
	 * 尋找前方方形範圍內，擁有特定標籤的最近敵人
	 * @param WorldContextObject
	 * @param Instigator       發起者
	 * @param TargetTag        目標必須擁有的標籤
	 * @param BoxTraceDistance       掃描距離
	 * @param TraceBoxSize        掃描範圍大小
	 * @param BoxTraceChannel	要掃描的Channel
	 * @param bDebugDraw       是否畫出除錯線
	 */
	UFUNCTION(BlueprintCallable, Category = "VoidFate|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static AActor* FindBestTargetHasGameplayTag(
		const UObject* WorldContextObject, 
		AActor* Instigator, 
		FGameplayTag TargetTag,
		const TArray<TEnumAsByte<EObjectTypeQuery>>& BoxTraceChannel,
		float BoxTraceDistance = 500.f, 
		FVector TraceBoxSize = FVector(500.f,500.f,300.f),
		bool bDebugDraw = false
	);

	UFUNCTION(BlueprintCallable, Category = "VoidFate|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static FVector CalculateIdealExecutionLocation(const UObject* WorldContextObject, AActor* Attacker, AActor* Victim);

	/** * 根據傳入的 InputAction 與輸入設備類型，回傳對應的 CommonUI 按鍵圖示
	 */
	UFUNCTION(BlueprintPure, Category = "VoidFate|UI", meta = (WorldContext = "WorldContextObject"))
	static FSlateBrush GetIconFromInputAction(const UObject* WorldContextObject, const UInputAction* InputAction, ECommonInputType InputType);

	/**
	 * 取得玩家當前正在使用的輸入設備類型 (鍵盤 或 手把)
	 */
	UFUNCTION(BlueprintPure, Category = "VoidFate|UI", meta = (WorldContext = "WorldContextObject"))
	static ECommonInputType GetCurrentInputType(const UObject* WorldContextObject);

	/** 根據當前的遊戲難度設定，自動開啟對應的關卡地圖 */
	UFUNCTION(BlueprintCallable, Category = "VoidFate|Level", meta = (WorldContext = "WorldContextObject"))
	static void OpenLevelBasedOnDifficulty(const UObject* WorldContextObject);
};

	