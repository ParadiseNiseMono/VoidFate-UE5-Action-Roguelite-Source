// Paradise NiseMono All Rights Reserved


#include "AnimInstances/AnimNotifies/VFAnimNotify_PostProcessFlash.h"
#include "Subsystems/VFCombatFeedbackSubsystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/VFCameraModifier_PostProcess.h"
#include "Engine/World.h"
#include "VFFunctionLibrary.h"
#include "VFGameplayTags.h"

UVFAnimNotify_PostProcessFlash::UVFAnimNotify_PostProcessFlash()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(192, 0, 192, 255);
#endif
}

void UVFAnimNotify_PostProcessFlash::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetWorld()) return;

	if (!bShouldPlayWhenVoidTime && UVFFunctionLibrary::NativeDoesActorHaveTag(MeshComp->GetOwner(), VFGameplayTags::Player_Status_VoidTime)) return;

	if (UVFCombatFeedbackSubsystem* CombatFeedback = MeshComp->GetWorld()->GetSubsystem<UVFCombatFeedbackSubsystem>())
	{
		if (ModifierClass && Material && StrengthCurve)
		{
			CombatFeedback->PlayPostProcessFlash(ModifierClass, Material, StrengthCurve);
		}
	}
}

FString UVFAnimNotify_PostProcessFlash::GetNotifyName_Implementation() const
{
	if (Material)
	{
		return FString::Printf(TEXT("PP Flash: %s"), *Material->GetName());
	}
	return TEXT("PP Flash (Empty)");
}
