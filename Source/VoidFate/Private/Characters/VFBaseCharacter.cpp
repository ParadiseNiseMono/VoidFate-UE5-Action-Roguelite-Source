// Paradise NiseMono All Rights Reserved


#include "Characters/VFBaseCharacter.h"

#include "AbilitySystem/VFAbilitySystemComponent.h"
#include "AbilitySystem/VFAttributeSet.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "MotionWarpingComponent.h"
#include "VFFunctionLibrary.h"
#include "VFGameplayTags.h"
#include "Components/BoxComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Items/Weapons/VFWeaponBase.h"
#include "Components/VFGhostTrailComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"

// Sets default values
AVFBaseCharacter::AVFBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	VFAbilitySystemComponent = CreateDefaultSubobject<UVFAbilitySystemComponent>(TEXT("VFAbilitySystemComponent"));
	VFAttributeSet = CreateDefaultSubobject<UVFAttributeSet>(TEXT("VFAttributeSet"));

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("LeftHandCollisionBox");
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnBodyCollisionBoxBeginOverlap);

	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("RightHandCollisionBox");
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnBodyCollisionBoxBeginOverlap);

	LeftFootCollisionBox = CreateDefaultSubobject<UBoxComponent>("LeftFootCollisionBox");
	LeftFootCollisionBox->SetupAttachment(GetMesh());
	LeftFootCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftFootCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnBodyCollisionBoxBeginOverlap);

	RightFootCollisionBox = CreateDefaultSubobject<UBoxComponent>("RightFootCollisionBox");
	RightFootCollisionBox->SetupAttachment(GetMesh());
	RightFootCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightFootCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnBodyCollisionBoxBeginOverlap);

	VFGhostTrailComponent = CreateDefaultSubobject<UVFGhostTrailComponent>(TEXT("VFGhostTrailComponent"));
}

void AVFBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (VFAbilitySystemComponent)
	{
		VFAbilitySystemComponent->InitAbilityActorInfo(this, this);

		ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Forgot to assign start up data to %s"), *GetName());
	}
}

void AVFBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (VFAbilitySystemComponent)
	{
		VFAbilitySystemComponent->RegisterGameplayTagEvent(
			VFGameplayTags::Shared_Status_Stunned
		).AddUObject(this, &ThisClass::OnStunTagChanged);

		VFAbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::OnPoiseStackEffectApplied);
	}
}

void AVFBaseCharacter::OnPoiseStackEffectApplied(UAbilitySystemComponent* Source, const FGameplayEffectSpec& SpecActive, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer GrantedTags;
	SpecActive.GetAllGrantedTags(GrantedTags);

	if (GrantedTags.HasTag(VFGameplayTags::Shared_Status_PoiseStack))
	{
		int32 CurrentStacks = VFAbilitySystemComponent->GetTagCount(VFGameplayTags::Shared_Status_PoiseStack);

		if (CurrentStacks >= PoiseThreshold)
		{
			FGameplayTagContainer TagContainer(VFGameplayTags::Shared_Status_PoiseStack);
			VFAbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(TagContainer);

			FGameplayEventData Payload;
			Payload.Target = this;

			UVFFunctionLibrary::RemoveGameplayTagFromActorIfFound(this, VFGameplayTags::Shared_Status_PoiseStack);

			if (Source)
			{
				Payload.Instigator = Source->GetAvatarActor();
			}

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
				this,
				VFGameplayTags::Shared_Event_HitReact,
				Payload
			);
		}
	}
}

void AVFBaseCharacter::OnStunTagChanged(const FGameplayTag StunTag, int32 NewCount)
{
	
}

void AVFBaseCharacter::SetCharacterFocusState(bool bIsFocused)
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp) return;

	MeshComp->SetRenderCustomDepth(bIsFocused);

	MeshComp->SetCustomDepthStencilValue(bIsFocused ? 1 : 0);
}

void AVFBaseCharacter::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (UVFFunctionLibrary::IsTargetPawnHostile(this,HitPawn))
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
			GetPawnCombatComponent()->OnHitTargetActor(HitPawn, FinalHit);
		}
	}
}

UAbilitySystemComponent* AVFBaseCharacter::GetAbilitySystemComponent() const
{
	return VFAbilitySystemComponent;
}

UPawnCombatComponent* AVFBaseCharacter::GetPawnCombatComponent() const
{
	return nullptr;
}

UPawnUIComponent* AVFBaseCharacter::GetPawnUIComponent() const
{
	return nullptr;
}

void AVFBaseCharacter::SetExecutionCollisionState(bool bIsExecuting)
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (!Capsule) return;

	if (bIsExecuting)
	{
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

		Capsule->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	}
	else
	{
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		Capsule->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
	}
}

#if WITH_EDITOR
void AVFBaseCharacter::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass,LeftHandCollisionBoxAttachBoneName))
	{
		LeftHandCollisionBox->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,LeftHandCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass,RightHandCollisionBoxAttachBoneName))
	{
		RightHandCollisionBox->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,RightHandCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass,LeftFootCollisionBoxAttachBoneName))
	{
		LeftFootCollisionBox->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,LeftFootCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass,RightFootCollisionBoxAttachBoneName))
	{
		RightFootCollisionBox->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,RightFootCollisionBoxAttachBoneName);
	}
}
#endif


