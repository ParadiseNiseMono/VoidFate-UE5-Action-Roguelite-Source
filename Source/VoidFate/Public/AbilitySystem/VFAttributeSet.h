// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "VFAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class IPawnUIInterface;
/**
 * 
 */
UCLASS()
class VOIDFATE_API UVFAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UVFAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// 處理數值限制 (Clamp) 的地方
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    
	// 處理戰鬥邏輯 (傷害計算、破防判定) 的地方
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category = "Vitality")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UVFAttributeSet, CurrentHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Vitality")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UVFAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Vitality")
	FGameplayAttributeData CurrentStamina;
	ATTRIBUTE_ACCESSORS(UVFAttributeSet, CurrentStamina)

	UPROPERTY(BlueprintReadOnly, Category = "Vitality")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UVFAttributeSet, MaxStamina)

	UPROPERTY(BlueprintReadOnly, Category = "Posture")
	FGameplayAttributeData CurrentPosture;
	ATTRIBUTE_ACCESSORS(UVFAttributeSet, CurrentPosture)

	UPROPERTY(BlueprintReadOnly, Category = "Posture")
	FGameplayAttributeData MaxPosture;
	ATTRIBUTE_ACCESSORS(UVFAttributeSet, MaxPosture)

	UPROPERTY(BlueprintReadOnly, Category = "Posture")
	FGameplayAttributeData PostureRecovery;
	ATTRIBUTE_ACCESSORS(UVFAttributeSet, PostureRecovery)

	UPROPERTY(BlueprintReadOnly, Category = "Posture")
	FGameplayAttributeData PostureDamage;
	ATTRIBUTE_ACCESSORS(UVFAttributeSet, PostureDamage)

	UPROPERTY(BlueprintReadOnly, Category = "AttackPower")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UVFAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UVFAttributeSet, Defense)

	UPROPERTY(BlueprintReadOnly, Category = "Cost")
	FGameplayAttributeData DodgeCost;
	ATTRIBUTE_ACCESSORS(UVFAttributeSet, DodgeCost)
	
	UPROPERTY(BlueprintReadOnly, Category = "Meta")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UVFAttributeSet, IncomingDamage)

private:
	TWeakInterfacePtr<IPawnUIInterface> CachedPawnUIInterface;

	void HandlePostureBreak(UAbilitySystemComponent* TargetASC);
};
