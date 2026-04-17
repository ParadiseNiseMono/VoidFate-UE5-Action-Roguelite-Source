// Paradise NiseMono All Rights Reserved


#include "Items/VFProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "VFDebugHelper.h"
#include "VFFunctionLibrary.h"
#include "VFGameplayTags.h"
#include "Abilities/GameplayAbilityTypes.h"

AVFProjectileBase::AVFProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBox"));
	SetRootComponent(ProjectileCollisionBox);
	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	ProjectileCollisionBox->OnComponentHit.AddUniqueDynamic(this,&ThisClass::OnProjectileHit);
	ProjectileCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnProjectileBeginOverlap);

	ProjectileNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagaraComponent"));
	ProjectileNiagaraComponent->SetupAttachment(GetRootComponent());

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 700.f;
	ProjectileMovementComp->MaxSpeed = 900.f;
	ProjectileMovementComp->Velocity = FVector(1.f,0.f,0.f);
	ProjectileMovementComp->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 4.f;
}

void AVFProjectileBase::SuspendProjectile_Implementation()
{
	if (ProjectileMovementComp)
	{
		ProjectileMovementComp->Velocity = FVector::ZeroVector;
		ProjectileMovementComp->ProjectileGravityScale = 0.0f;
		ProjectileMovementComp->bIsHomingProjectile = false;
	}
}

void AVFProjectileBase::FireProjectile_Implementation(AActor* TargetActor)
{
	if (ProjectileMovementComp && TargetActor)
	{
		ProjectileMovementComp->HomingTargetComponent = TargetActor->GetRootComponent();
		ProjectileMovementComp->bIsHomingProjectile = true;
	}
}

void AVFProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileDamagePolicy == EProjectileDamagePolicy::OnBeginOverlap)
	{
		ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
		ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
		ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	}

	if (AActor* InstigatorActor = GetInstigator())
	{
		ProjectileCollisionBox->IgnoreActorWhenMoving(InstigatorActor, true);
	}
	if (AActor* OwnerActor = GetOwner())
	{
		ProjectileCollisionBox->IgnoreActorWhenMoving(OwnerActor, true);
	}
}

void AVFProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	BP_OnSpawnProjectileHitFX(Hit.ImpactPoint);
	
	APawn* HitPawn = Cast<APawn>(OtherActor);

	if (HitPawn == GetOwner())
	{
		return;
	}

	if (!HitPawn || !UVFFunctionLibrary::IsTargetPawnHostile(GetInstigator(),HitPawn))
	{
		Destroy();
		return;
	}

	bool bIsValidBlock = false;

	const bool IsPlayerBlocking = UVFFunctionLibrary::NativeDoesActorHaveTag(HitPawn, VFGameplayTags::Player_Status_Blocking);
	const bool bIsPlayerDodging = UVFFunctionLibrary::NativeDoesActorHaveTag(HitPawn, VFGameplayTags::Player_Status_Dodging);

	if (IsPlayerBlocking)
	{
		bIsValidBlock = UVFFunctionLibrary::IsValidBlock(this, HitPawn);
	}

	FGameplayEventData Data = UVFFunctionLibrary::MakeGameplayEventDataWithHitResult(this, HitPawn, Hit);
	
	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitPawn,
			VFGameplayTags::Player_Event_SuccessfulBlock,
			Data
		);
	}
	else if (bIsPlayerDodging)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitPawn,
			VFGameplayTags::Player_Event_PerfectDodge,
			UVFFunctionLibrary::MakeGameplayEventDataWithHitResult(GetOwner(), HitPawn, Hit)
		);
	}
	else
	{
		HandleApplyProjectileDamage(HitPawn, Data);
	}

	Destroy();
}

void AVFProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedActors.Contains(OtherActor))
	{
		return;
	}

	OverlappedActors.AddUnique(OtherActor);

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		FGameplayEventData Data = UVFFunctionLibrary::MakeGameplayEventDataWithHitResult(this, HitPawn, SweepResult);
		Data.Instigator = GetInstigator();
		Data.Target = HitPawn;

		if (UVFFunctionLibrary::IsTargetPawnHostile(GetInstigator(),HitPawn))
		{
			HandleApplyProjectileDamage(HitPawn,Data);
		}
	}
}

void AVFProjectileBase::HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload)
{
	checkf(ProjectileDamageEffectSpecHandle.IsValid(),TEXT("Forgot to assign a valid spec handle to the projectile: %s"),*GetActorNameOrLabel());

	UVFFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(),InHitPawn,ProjectileDamageEffectSpecHandle);
}


