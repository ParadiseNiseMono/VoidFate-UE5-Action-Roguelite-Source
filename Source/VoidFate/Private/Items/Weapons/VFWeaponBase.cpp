// Paradise NiseMono All Rights Reserved


#include "Items/Weapons/VFWeaponBase.h"

#include "VFDebugHelper.h"
#include "VFFunctionLibrary.h"
#include "Components/BoxComponent.h"

// Sets default values
AVFWeaponBase::AVFWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);
}

void AVFWeaponBase::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	checkf(WeaponOwningPawn,TEXT("Forgot to assign an instigator as the owning pawn for the weapon: %s"),*GetName());

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (UVFFunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
		{
			FHitResult FinalHit = SweepResult;
			
			if (!bFromSweep || FinalHit.ImpactPoint.IsZero())
			{
				FVector WeaponLocation = GetActorLocation();
				FVector TargetClosestPoint;
               
	
				OtherComp->GetClosestPointOnCollision(WeaponLocation, TargetClosestPoint);
				
				if (TargetClosestPoint.IsZero())
				{
					TargetClosestPoint = OtherActor->GetActorLocation();
				}

				FinalHit.ImpactPoint = TargetClosestPoint;
				FinalHit.Location = TargetClosestPoint;

				FinalHit.ImpactNormal = (WeaponLocation - TargetClosestPoint).GetSafeNormal();
			}
			OnWeaponHitTarget.ExecuteIfBound(OtherActor, FinalHit);
		}
	}
}

void AVFWeaponBase::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	checkf(WeaponOwningPawn,TEXT("Forgot to assign an instigator as the owning pawn of the weapon: %s"),*GetName());

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (UVFFunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
		{
			OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor, FHitResult());
		}

	}
}

