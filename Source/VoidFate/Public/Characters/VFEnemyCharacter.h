// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/VFBaseCharacter.h"
#include "Components/WidgetComponent.h"
#include "VFEnemyCharacter.generated.h"

struct FGameplayTag;
class UVFStatBarWidget;
struct FOnAttributeChangeData;
class UWidgetComponent;
class UEnemyUIComponent;
class UEnemyCombatComponent;

/**
 * 
 */
UCLASS()
class VOIDFATE_API AVFEnemyCharacter : public AVFBaseCharacter
{
	GENERATED_BODY()

public:
	AVFEnemyCharacter();

	//~ Begin IPawnCombatInterface	Interface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface

	//~ Begin IPawnUIInterface	Interface
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UEnemyUIComponent* GetEnemyUIComponent() const override;
	//~ End IPawnUIInterface Interface

protected:
	//~ Begin APawn	Interface
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	//~ End APawn Interface

	//~ Begin AVFBaseCharacter Interface
	virtual void OnStunTagChanged(const FGameplayTag StunTag, int32 NewCount) override;
	//~ End AVFBaseCharacter Interface
	
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UEnemyCombatComponent* EnemyCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UEnemyUIComponent* EnemyUIComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* HealthBarWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* PostureBarWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Void|Combat|Execution")
	USceneComponent* ExecutionStandPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	int32 EnemyLevel = 1;

	FTimerHandle HideStateBarTimerHandle;

	UPROPERTY(EditAnywhere, Category = "UI")
	float HideStateBarDuration = 4.0f;
private:
	void InitEnemyStartUpData();

public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const {return EnemyCombatComponent;}
	FORCEINLINE void SetEnemyLevel(const int32 InLevel) {EnemyLevel = InLevel;}

	UFUNCTION(BlueprintCallable, Category = "Void|Combat|Execution")
	FTransform GetExecutionStandPointTransform() const;
};
