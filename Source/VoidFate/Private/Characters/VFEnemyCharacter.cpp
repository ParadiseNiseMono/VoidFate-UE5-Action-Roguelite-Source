// Paradise NiseMono All Rights Reserved


#include "Characters/VFEnemyCharacter.h"

#include "AbilitySystemComponent.h"
#include "VFDebugHelper.h"
#include "VFFunctionLibrary.h"
#include "VFGameplayTags.h"
#include "AbilitySystem/VFAbilitySystemComponent.h"
#include "AbilitySystem/VFAttributeSet.h"
#include "AbilitySystem/Abilities/VFEnemyGameplayAbility.h"
#include "Components/BoxComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Components/UI/EnemyUIComponent.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widgets/VFStatBarWidget.h"

AVFEnemyCharacter::AVFEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,180.f,0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("EnemyCombatComponent"));

	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>(TEXT("EnemyUIComponent"));
	
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComp"));
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetDrawSize(FVector2D(150.0f, 20.0f));
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f));

	PostureBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PostureBarComp"));
	PostureBarWidgetComponent->SetupAttachment(GetRootComponent());
	PostureBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	PostureBarWidgetComponent->SetDrawSize(FVector2D(150.0f, 20.0f));
	PostureBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));

	ExecutionStandPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ExecutionStandPoint"));

	ExecutionStandPoint->SetupAttachment(GetMesh());

	ExecutionStandPoint->SetRelativeLocation(FVector(0.f, 100.f, 0.f));
	ExecutionStandPoint->SetRelativeRotation(FRotator(0.f, 270.f, 0.f));
}

UPawnCombatComponent* AVFEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

UPawnUIComponent* AVFEnemyCharacter::GetPawnUIComponent() const
{
	return EnemyUIComponent;
}

UEnemyUIComponent* AVFEnemyCharacter::GetEnemyUIComponent() const
{
	return EnemyUIComponent;
}

void AVFEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartUpData();
}

void AVFEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UVFStatBarWidget* HealthBar = Cast<UVFStatBarWidget>(HealthBarWidgetComponent->GetUserWidgetObject()))
	{
		HealthBar->InitEnemyCreatedWidget(this);
	}
	if (UVFStatBarWidget* PostureBarWidget = Cast<UVFStatBarWidget>(PostureBarWidgetComponent->GetUserWidgetObject()))
	{
		PostureBarWidget->InitEnemyCreatedWidget(this);
	}
}

void AVFEnemyCharacter::OnStunTagChanged(const FGameplayTag StunTag, int32 NewCount)
{
	if (NewCount == 0 && !UVFFunctionLibrary::NativeDoesActorHaveTag(this, VFGameplayTags::Shared_Status_Dead) && VFAttributeSet && VFAbilitySystemComponent)
	{
		UVFAttributeSet* AttributeSet = VFAttributeSet;
		float HalfPosture = AttributeSet->GetMaxPosture() * 0.5f;

		VFAbilitySystemComponent->ApplyModToAttributeUnsafe(UVFAttributeSet::GetCurrentPostureAttribute(), EGameplayModOp::Override, HalfPosture);
		UVFFunctionLibrary::RemoveGameplayTagFromActorIfFound(this, VFGameplayTags::Shared_Status_Posture_Full);
	}
}


void AVFEnemyCharacter::InitEnemyStartUpData()
{
	if (CharacterStartUpData.IsNull()) return;
	TWeakObjectPtr<AVFEnemyCharacter> WeakThis = this;

	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this, WeakThis]()
			{
				if (AVFEnemyCharacter* StrongThis = WeakThis.Get()) {
					CharacterStartUpData->GiveToAbilitySystemComponent(VFAbilitySystemComponent, EnemyLevel);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("StartUpData 載入完成，但敵人已經死亡，取消賦予技能。"));
				}
			}
		)
	);
}

FTransform AVFEnemyCharacter::GetExecutionStandPointTransform() const
{
	if (ExecutionStandPoint)
	{
		return ExecutionStandPoint->GetComponentTransform();
	}
	return GetActorTransform();
}
