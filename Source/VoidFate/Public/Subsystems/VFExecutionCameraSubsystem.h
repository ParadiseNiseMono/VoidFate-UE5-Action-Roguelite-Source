// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VFExecutionCameraSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossIntroSignature, const FText&, BossName);

/**
 * 專門負責處決運鏡的導演子系統
 * 負責計算最佳鏡位、避開牆壁、並執行視角切換
 */
UCLASS()
class VOIDFATE_API UVFExecutionCameraSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	// Offset: 攝影機相對於處決中心點的偏移 (例如：X=150, Y=150, Z=50)
	// BlendDuration: 切換鏡頭的過渡時間 (0.0 = 瞬間切換/Jump Cut)
	UFUNCTION(BlueprintCallable, Category = "Void|Camera|Execution")
	void StartCinematicExecution(AActor* Attacker, AActor* Victim, FVector CameraOffset, float BlendDuration = 0.2f);

	UFUNCTION(BlueprintCallable, Category = "Void|Camera|Execution")
	void StopCinematicExecution(float BlendDuration = 0.4f);

	UPROPERTY(BlueprintAssignable, Category = "VF|Cinematic")
	FOnBossIntroSignature OnBossIntroTriggered;

	// 開始 Boss 特寫
	UFUNCTION(BlueprintCallable, Category = "VF|Cinematic")
	void StartBossIntro(AActor* BossActor, FText BossAbstractName, float IntroDuration = 6.f);

	// 結束特寫並恢復時間
	UFUNCTION(BlueprintCallable, Category = "VF|Cinematic")
	void StopBossIntro();

protected:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

private:
	// 計算最佳攝影機位置 (包含防穿牆邏輯)
	FVector CalculateBestCameraLocation(AActor* Attacker, AActor* Victim, FVector IdealOffset);

	// 我們專屬的「處決攝影機」
	UPROPERTY(Transient)
	TObjectPtr<ACameraActor> CinematicCameraActor;

	// 暫存原本的玩家控制器，方便切換回來
	UPROPERTY(Transient)
	TObjectPtr<APlayerController> TargetPC;

	FTimerHandle BossIntroTimerHandle;
};
