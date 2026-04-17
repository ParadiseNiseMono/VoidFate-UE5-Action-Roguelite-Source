// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VFCombatFeedbackSubsystem.generated.h"

/**
 * 掌管全域戰鬥視覺與聽覺回饋的子系統
 * 職責包含：頓幀(Hit Stop)、攝影機震動、FOVs切換等
 */

class UVFCameraModifier_FOVImpact;

UCLASS()
class VOIDFATE_API UVFCombatFeedbackSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Void|Combat|Feedback")
	void ApplyHitStop(AActor* TargetActor, float Duration = 0.05f, float TimeDilation = 0.01f);

	UFUNCTION(BlueprintCallable, Category = "Void|Combat|Feedback")
	void ApplyHitStopToBoth(AActor* Attacker, AActor* Defender, float Duration = 0.1f, float TimeDilation = 0.01f);

	UFUNCTION(BlueprintCallable, Category = "Void|Combat|Feedback")
	void PlayFOVImpact(APlayerController* PC, TSubclassOf<UVFCameraModifier_FOVImpact> ModifierClass, float FOVOffset = -15.0f, float Duration = 0.15f, UCurveFloat* ImpactCurve = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Void|Combat|Feedback")
	void PlayPostProcessFlash(TSubclassOf<class UVFCameraModifier_PostProcess> ModifierClass, UMaterialInterface* Material, UCurveFloat* StrengthCurve);

	// 強制中斷並清除所有正在進行中的 Hit Stop (適用於進入特殊狀態前重置時間)
	UFUNCTION(BlueprintCallable, Category = "Void|Combat|Feedback")
	void StopAllHitStops();

	/**
	 * 播放指向性攝影機震動
	 * @param HitDirection - 攻擊的受力方向 (例如：從攻擊者指向防禦者的向量)
	 * @param Scale - 震動強度倍率
	 */
	UFUNCTION(BlueprintCallable, Category = "Void|Combat|Feedback")
	void PlayDirectionalCameraShake(APlayerController* PC, TSubclassOf<class UCameraShakeBase> ShakeClass, FVector HitDirection, float Scale = 1.0f);

	//開啟常駐濾鏡，並回傳指標供後續關閉使用
	UFUNCTION(BlueprintCallable, Category = "Void|Combat|Feedback")
	UVFCameraModifier_PostProcess* AddPersistentPostProcess(TSubclassOf<class UVFCameraModifier_PostProcess> ModifierClass, UMaterialInterface* Material);

	//手動關閉濾鏡
	UFUNCTION(BlueprintCallable, Category = "Void|Combat|Feedback")
	void RemovePersistentPostProcess(UVFCameraModifier_PostProcess* Modifier);
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	// 負責把 Actor 的時間恢復正常的內部函式
	void RestoreTimeDilation(TWeakObjectPtr<AActor> WeakActor);

	// 【核心防呆】記錄每個 Actor 目前的 Hit Stop 計時器
	// 使用 TWeakObjectPtr 防止 Actor 死亡時卡住記憶體
	TMap<TWeakObjectPtr<AActor>, FTimerHandle> ActiveHitStopTimers;
};
