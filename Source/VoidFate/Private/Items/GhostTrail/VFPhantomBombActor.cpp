// Paradise NiseMono All Rights Reserved


#include "Items/GhostTrail/VFPhantomBombActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h" 
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystem/VFAbilitySystemComponent.h"
#include "VFGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "VFFunctionLibrary.h"
#include "Components/PoseableMeshComponent.h"

AVFPhantomBombActor::AVFPhantomBombActor()
{
	DetonationTime = 1.0f;
	ExplosionRadius = 300.0f;

	if (PoseableMesh)
	{
		PoseableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		PoseableMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

		PoseableMesh->SetGenerateOverlapEvents(false);
	}
}

void AVFPhantomBombActor::InitBomb()
{
	InitGhost(PoseSource, BombMaterial, 0.f);

	if (bAutoExplode) {
		GetWorld()->GetTimerManager().SetTimer(DetonationTimerHandle, this, &AVFPhantomBombActor::Explode, DetonationTime, false);
	}
}

void AVFPhantomBombActor::BeginPlay()
{
	Super::BeginPlay();

	InitBomb();
}

void AVFPhantomBombActor::Explode()
{
	if (InstigatorVFASC && ExplosionCueTag.IsValid())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = GetActorLocation();
		InstigatorVFASC->ExecuteGameplayCue(ExplosionCueTag, CueParams);
	}

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	if (InstigatorVFASC)
	{
		IgnoreActors.Add(InstigatorVFASC->GetOwner());
	}

	TArray<AActor*> OverlappedActors;
	UKismetSystemLibrary::SphereOverlapActors(
		this,
		GetActorLocation(),
		ExplosionRadius,
		TargetObjectTypes,
		nullptr,
		IgnoreActors,
		OverlappedActors
	);

	if (InstigatorVFASC && ExplosionEffects.Num() > 0)
	{
		for (AActor* HitActor : OverlappedActors)
		{
			UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(HitActor);
			if (TargetASC)
			{
				FGameplayEffectContextHandle ContextHandle = InstigatorVFASC->MakeEffectContext();
				ContextHandle.AddInstigator(InstigatorVFASC->GetOwner(), InstigatorVFASC->GetOwner());

				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
					HitActor,
					VFGameplayTags::Shared_Event_HitReact,
					UVFFunctionLibrary::MakeGameplayEventDataWithHitResult(GetOwner(), HitActor, FHitResult())
						);

				for (TSubclassOf<UGameplayEffect> EffectClass : ExplosionEffects)
				{
					if (EffectClass)
					{
						FGameplayEffectSpecHandle SpecHandle = InstigatorVFASC->MakeOutgoingSpec(EffectClass, 1.0f, ContextHandle);
						SpecHandle.Data->SetSetByCallerMagnitude(
							VFGameplayTags::Shared_SetByCaller_BaseDamage,
							ExplosionBaseDamage
						);
						if (SpecHandle.IsValid())
						{
							InstigatorVFASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
						}
					}
				}
			}
		}
	}

	if (bShowDebugSphere)
	{
		DrawDebugSphere(
			GetWorld(),
			GetActorLocation(),
			ExplosionRadius,
			32,					
			FColor::Red,		
			false,				
			DebugSphereDuration,
			0,					
			5.0f				
		);
	}

	Destroy();
}
