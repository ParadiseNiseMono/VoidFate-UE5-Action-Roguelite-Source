// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"
#include "VFBaseCharacter.generated.h"

struct FGameplayTag;
class UBoxComponent;
class UDataAsset_StartUpDataBase;
class UVFAttributeSet;
class UVFAbilitySystemComponent;
class UMotionWarpingComponent;
class UVFGhostTrailComponent;
struct FGameplayEffectSpec;
struct FActiveGameplayEffectHandle;

UCLASS()
class VOIDFATE_API AVFBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IPawnCombatInterface, public IPawnUIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVFBaseCharacter();

	//~ Begin IAbilitySystemInterface	Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface

	//~ Begin IPawnCombatInterface	Interface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface

	//~ Begin IPawnUIInterface	Interface
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	//~ End IPawnUIInterface Interface


	UFUNCTION(BlueprintCallable, Category = "Void|Combat")
	void SetExecutionCollisionState(bool bIsExecuting);

#if WITH_EDITOR
	//~ Begin UObject Interface.
	virtual void PostEditChangeProperty( struct FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End UObject Interface
#endif
	
protected:
	//~ Begin APawn	Interface
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface
	virtual void BeginPlay() override;

	virtual void OnPoiseStackEffectApplied(UAbilitySystemComponent* Source, const FGameplayEffectSpec& SpecActive, FActiveGameplayEffectHandle ActiveHandle);

	UFUNCTION()
	virtual void OnStunTagChanged(const FGameplayTag StunTag, int32 NewCount);

	UFUNCTION(BlueprintCallable, Category = "Void|Visuals")
	void SetCharacterFocusState(bool bIsFocused);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	UVFAbilitySystemComponent* VFAbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	UVFAttributeSet* VFAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion Warping")
	UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBoxComponent* LeftHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName LeftHandCollisionBoxAttachBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBoxComponent* RightHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName RightHandCollisionBoxAttachBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBoxComponent* LeftFootCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName LeftFootCollisionBoxAttachBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBoxComponent* RightFootCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UVFGhostTrailComponent* VFGhostTrailComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName RightFootCollisionBoxAttachBoneName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Data", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

	UFUNCTION()
	virtual void OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 霸體忍耐度 (需要幾層才會硬直)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	int32 PoiseThreshold = 3;

public:
	FORCEINLINE UVFAbilitySystemComponent* GetVFAbilitySystemComponent() const { return VFAbilitySystemComponent; }

	FORCEINLINE UVFAttributeSet* GetVFAttributeSet() const { return VFAttributeSet; }
	
	FORCEINLINE UBoxComponent* GetLeftHandCollisionBox() const {return LeftHandCollisionBox;}
	FORCEINLINE UBoxComponent* GetRightHandCollisionBox() const {return RightHandCollisionBox;}
	FORCEINLINE UBoxComponent* GetLeftFootCollisionBox() const {return LeftFootCollisionBox;}
	FORCEINLINE UBoxComponent* GetRightFootCollisionBox() const {return RightFootCollisionBox;}

	FORCEINLINE UMotionWarpingComponent* GetMotionWarpingComponent() const { return MotionWarpingComponent; }
};
