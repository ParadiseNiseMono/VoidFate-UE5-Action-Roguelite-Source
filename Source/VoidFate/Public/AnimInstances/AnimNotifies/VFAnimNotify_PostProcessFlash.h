// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "VFAnimNotify_PostProcessFlash.generated.h"

class UVFCameraModifier_PostProcess;
class UMaterialInterface;
class UCurveFloat;

/**
 * 專門用來在動畫時間軸上觸發全螢幕濾鏡/爆發特效的 Notify
 */
UCLASS(meta = (DisplayName = "Play Post Process Flash"))
class VOIDFATE_API UVFAnimNotify_PostProcessFlash : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UVFAnimNotify_PostProcessFlash();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual FString GetNotifyName_Implementation() const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
	TSubclassOf<UVFCameraModifier_PostProcess> ModifierClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
	UCurveFloat* StrengthCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
	bool bShouldPlayWhenVoidTime = false;
};
