// Paradise NiseMono All Rights Reserved


#include "VFFunctionLibrary.h"

#include "AbilitySystem/VFAbilitySystemComponent.h"
#include "VoidFateSettings/VFDeveloperSettings.h"
#include "VoidFateSettings/VFGameUserSettings.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "VFDebugHelper.h"
#include "VFGameplayTags.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Internationalization/StringTable.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LatentActions/FVFCountDownAction.h"
#include "VFGameInstance.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "InputAction.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "CommonInputSubsystem.h"
#include "Engine/AssetManager.h"

TSoftClassPtr<UWidget_ActivatableBase> UVFFunctionLibrary::GetFrontendSoftWidgetClassByTag(UPARAM(meta = (Categories = "Frontend.Widget")) FGameplayTag InWidgetTag)
{
	const UVFDeveloperSettings* DeveloperSettings = GetDefault<UVFDeveloperSettings>();

	checkf(DeveloperSettings->FrontWidgetBase.Contains(InWidgetTag), TEXT("FrontWidgetBase is not found in VFFunctionLibrary! Under the tag : %s"), *InWidgetTag.ToString());

	return DeveloperSettings->FrontWidgetBase.FindRef(InWidgetTag);
}

TSoftObjectPtr<UTexture2D> UVFFunctionLibrary::GetOptionsSoftImageByTag(UPARAM(meta = (Categories = "Frontend.Image")) FGameplayTag InImageTag)
{
	const UVFDeveloperSettings* DeveloperSettings = GetDefault<UVFDeveloperSettings>();

	checkf(DeveloperSettings->OptionsScreenSoftImageMap.Contains(InImageTag), TEXT("Could find an image associate with tag : %s"), *InImageTag.ToString());

	return DeveloperSettings->OptionsScreenSoftImageMap.FindRef(InImageTag);
}

UVFGameUserSettings* UVFFunctionLibrary::GetVFGameUserSettings()
{
	if (GEngine)
	{
		return CastChecked<UVFGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}

UVFAbilitySystemComponent* UVFFunctionLibrary::NativeGetVFASCFromActor(AActor* InActor)
{
	if (!IsValid(InActor))
	{
		return nullptr;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor);

	if (ASC)
	{
		return Cast<UVFAbilitySystemComponent>(ASC);
	}

	return nullptr;
}

void UVFFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UVFAbilitySystemComponent* VFASC = NativeGetVFASCFromActor(InActor);

	if (VFASC == nullptr)
	{
		return;
	}

	if (!VFASC->HasMatchingGameplayTag(TagToAdd))
	{
		VFASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UVFFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UVFAbilitySystemComponent* VFASC = NativeGetVFASCFromActor(InActor);

	if (VFASC == nullptr)
	{
		return;
	}

	if (VFASC->HasMatchingGameplayTag(TagToRemove))
	{
		VFASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UVFFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UVFAbilitySystemComponent* VFASC = NativeGetVFASCFromActor(InActor);
	
	if (VFASC == nullptr)
	{
		return false;
	}

	return VFASC->HasMatchingGameplayTag(TagToCheck);
}

void UVFFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EVFConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor,TagToCheck) ? EVFConfirmType::Yes : EVFConfirmType::No;
}

UPawnCombatComponent* UVFFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);

	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}
	return nullptr;
}

UPawnCombatComponent* UVFFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor,
	EVFValidType& OutValidType)
{
	UPawnCombatComponent* PawnCombatComponent = NativeGetPawnCombatComponentFromActor(InActor);

	OutValidType = PawnCombatComponent ? EVFValidType::Valid : EVFValidType::Invalid;

	return PawnCombatComponent;
}

bool UVFFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
	check(QueryPawn && TargetPawn);

	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}

	return false;
}

float UVFFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
	return InScalableFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UVFFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim,
	float& OutAngleDifference)
{
	check(InAttacker && InVictim);

	const FVector VictimForward = InVictim->GetActorForwardVector();
	const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

	const float DotResult = FVector::DotProduct(VictimForward,VictimToAttackerNormalized);
	OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

	const FVector CrossResult = FVector::CrossProduct(VictimForward,VictimToAttackerNormalized);

	if (CrossResult.Z < 0.f)
	{
		OutAngleDifference *= -1.f;
	}

	if (OutAngleDifference>=-45.f && OutAngleDifference <=45.f)
	{
		return VFGameplayTags::Shared_Status_HitReact_Front;
	}
	else if (OutAngleDifference<-45.f && OutAngleDifference>=-135.f)
	{
		return VFGameplayTags::Shared_Status_HitReact_Left;
	}
	else if (OutAngleDifference<-135.f || OutAngleDifference>135.f)
	{
		return VFGameplayTags::Shared_Status_HitReact_Back;
	}
	else if(OutAngleDifference>45.f && OutAngleDifference<=135.f)
	{
		return VFGameplayTags::Shared_Status_HitReact_Right;
	}

	return FGameplayTag();
}

bool UVFFunctionLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender)
{
	check(InAttacker && InDefender);

	const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(),InDefender->GetActorForwardVector());

	/*const FString DebugString = FString::Printf(TEXT("Dot Result: %f %s"),DotResult,DotResult<-0.5f? TEXT("Valid Block") : TEXT("InvalidBlock"));

	Debug::Print(DebugString);*/

	// 無論如何都成功格擋
	return DotResult<=1;
}

AActor* UVFFunctionLibrary::FindNearestTargetInRadius(const UObject* WorldContextObject, AActor* SourceActor,
	float SearchRadius, float SearchAngle, bool bDebugDraw)
{
	if (!SourceActor || !WorldContextObject) return nullptr;

    UWorld* World = WorldContextObject->GetWorld();
    if (!World) return nullptr;
	
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(SourceActor);

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

    TArray<AActor*> OutActors;
	
    UKismetSystemLibrary::SphereOverlapActors(
        World,
        SourceActor->GetActorLocation(),
        SearchRadius,
        ObjectTypes,
        nullptr,
        ActorsToIgnore,
        OutActors
    );
	
    AActor* BestTarget = nullptr;
    float MinDistSq = FLT_MAX;
    
    // 將角度轉換為 Cos 值 (例如 90度 = 0, 0度 = 1, 180度 = -1)
    // 注意：SearchAngle 是半角還是全角？這裡假設輸入 90 代表 "左右各 90 度" (即前半球)
    float CosHalfAngle = FMath::Cos(FMath::DegreesToRadians(SearchAngle));
    FVector SourceForward = SourceActor->GetActorForwardVector();
    FVector SourceLoc = SourceActor->GetActorLocation();

    for (AActor* Candidate : OutActors)
    {
        if (!Candidate) continue;
    	
        if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Candidate))
        {
            if (ASC->HasMatchingGameplayTag(VFGameplayTags::Shared_Status_Dead) ||
            	!IsTargetPawnHostile(Cast<APawn>(SourceActor), Cast<APawn>(Candidate))) continue;
        }
    	
        FVector ToTarget = (Candidate->GetActorLocation() - SourceLoc).GetSafeNormal();
        float DotResult = FVector::DotProduct(SourceForward, ToTarget);
    	
        if (DotResult < CosHalfAngle) continue;
    	
        float DistSq = FVector::DistSquared(SourceLoc, Candidate->GetActorLocation());
        if (DistSq < MinDistSq)
        {
            MinDistSq = DistSq;
            BestTarget = Candidate;
        }
    }
	
    if (bDebugDraw)
    {
        FColor DrawColor = BestTarget ? FColor::Green : FColor::Red;
        DrawDebugSphere(World, SourceLoc, SearchRadius, 12, FColor::Yellow, false, 2.0f);
        
        if (BestTarget)
        {
            DrawDebugLine(World, SourceLoc, BestTarget->GetActorLocation(), FColor::Green, false, 2.0f, 0, 2.0f);
            DrawDebugString(World, BestTarget->GetActorLocation(), TEXT("TARGET"), nullptr, FColor::White, 2.0f);
        }
    }

    return BestTarget;
}

bool UVFFunctionLibrary::GetCombatWarpLocation(AActor* Attacker, AActor* Target, float AttackRange,
	FVector& OutLocation, FRotator& OutRotation)
{
	if (!Attacker || !Target) return false;
	
	FVector AttackerLoc = Attacker->GetActorLocation();
	FVector TargetLoc = Target->GetActorLocation();
	
	FVector DirectionToTarget = (TargetLoc - AttackerLoc).GetSafeNormal2D();
	
	float TargetRadius = 0.0f;
	float AttackerRadius = 0.0f;
	
	TargetRadius = Target->GetSimpleCollisionRadius();
	AttackerRadius = Attacker->GetSimpleCollisionRadius();


	float IdealDistance = TargetRadius + AttackerRadius + (AttackRange * 0.8f);
	
	OutLocation = TargetLoc - (DirectionToTarget * IdealDistance);
	
	OutLocation.Z = AttackerLoc.Z;
	
	OutRotation = DirectionToTarget.Rotation();

	return true;
}

FGameplayEventData UVFFunctionLibrary::MakeGameplayEventDataWithHitResult(AActor* InInstigator, AActor* InTargetActor,
                                                                          const FHitResult& InHitResult)
{
	FGameplayEventData Data;
	Data.Instigator = InInstigator;
	Data.Target = InTargetActor;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InTargetActor);

	if (ASC)
	{
		FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
		
		Context.AddHitResult(InHitResult);
		
		Data.ContextHandle = Context;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find ASC on Owner!"));
	}
	return Data;
}

bool UVFFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor,
	const FGameplayEffectSpecHandle& InSpecHandle)
{
	UVFAbilitySystemComponent* SourceASC = NativeGetVFASCFromActor(InInstigator);
	UVFAbilitySystemComponent* TargetASC = NativeGetVFASCFromActor(InTargetActor);

	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data,TargetASC);

	return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}

void UVFFunctionLibrary::CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval,
	float& OutRemainingTime, EVFCountDownActionInput CountDownInput, EVFCountDownActionOutput& CountDownOutput,
	FLatentActionInfo LatentInfo)
{
	UWorld* World = nullptr;

	if (GEngine)
	{
		World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull);
	}

	if (!World)
	{
		return;
	}

	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();

	FVFCountDownAction* FoundAction = LatentActionManager.FindExistingAction<FVFCountDownAction>(LatentInfo.CallbackTarget,LatentInfo.UUID);

	if (CountDownInput == EVFCountDownActionInput::Start)
	{
		if (!FoundAction)
		{
			LatentActionManager.AddNewAction(
				LatentInfo.CallbackTarget,
				LatentInfo.UUID,
				new FVFCountDownAction(TotalTime, UpdateInterval, OutRemainingTime, CountDownOutput, LatentInfo)
			);
		}
		else
		{
			FoundAction->ResetAction(TotalTime, UpdateInterval);
		}
	}

	if (CountDownInput == EVFCountDownActionInput::Cancel)
	{
		if (FoundAction)
		{
			FoundAction->CancelAction();
		}
	}
}

FText UVFFunctionLibrary::GetOptionsScreenDescriptionText(FName Key)
{
	const UVFDeveloperSettings* Settings = GetDefault<UVFDeveloperSettings>();
	if (Settings && !Settings->OptionsScreenDescriptionStringTable.IsNull())
	{
		UStringTable* Table = Settings->OptionsScreenDescriptionStringTable.LoadSynchronous();
		if (Table)
		{
			return FText::FromStringTable(Table->GetStringTableId(), Key.ToString());
		}
	}
	return FText::GetEmpty();
}

FText UVFFunctionLibrary::GetOptionsScreenTitleText(FName Key)
{
	const UVFDeveloperSettings* Settings = GetDefault<UVFDeveloperSettings>();
	if (Settings && !Settings->OptionsScreenTitleStringTable.IsNull())
	{
		UStringTable* Table = Settings->OptionsScreenTitleStringTable.LoadSynchronous();
		if (Table)
		{
			return FText::FromStringTable(Table->GetStringTableId(), Key.ToString());
		}
	}
	return FText::GetEmpty();
}

FText UVFFunctionLibrary::GetModalScreenTitleText(FName Key)
{
	const UVFDeveloperSettings* Settings = GetDefault<UVFDeveloperSettings>();
	if (Settings && !Settings->ModalScreenTitleStringTable.IsNull())
	{
		UStringTable* Table = Settings->ModalScreenTitleStringTable.LoadSynchronous();
		if (Table)
		{
			return FText::FromStringTable(Table->GetStringTableId(), Key.ToString());
		}
	}
	return FText::GetEmpty();
}

FText UVFFunctionLibrary::GetPlayerStateScreenText(FName Key)
{
	const UVFDeveloperSettings* Settings = GetDefault<UVFDeveloperSettings>();
	if (Settings && !Settings->PlayerStateScreenStringTable.IsNull())
	{
		UStringTable* Table = Settings->PlayerStateScreenStringTable.LoadSynchronous();
		if (Table)
		{
			return FText::FromStringTable(Table->GetStringTableId(), Key.ToString());
		}
	}
	return FText::GetEmpty();
}

FText UVFFunctionLibrary::GetLoadingScreenText(FName Key)
{
	const UVFDeveloperSettings* Settings = GetDefault<UVFDeveloperSettings>();
	if (Settings && !Settings->LoadingScreenStringTable.IsNull())
	{
		UStringTable* Table = Settings->LoadingScreenStringTable.LoadSynchronous();
		if (Table)
		{
			return FText::FromStringTable(Table->GetStringTableId(), Key.ToString());
		}
	}
	return FText::GetEmpty();
}

USoundBase* UVFFunctionLibrary::GetBGMByGameDifficulty(EVFGameDifficulty InDifficulty)
{
	const UVFDeveloperSettings* Settings = GetDefault<UVFDeveloperSettings>();
	if (!Settings) return nullptr;

	switch (InDifficulty)
	{
	case EVFGameDifficulty::Easy:
		return Settings->EasyModeBGM.LoadSynchronous();

	case EVFGameDifficulty::Normal:
		return Settings->NormalModeBGM.LoadSynchronous();

	case EVFGameDifficulty::Hard:
		return Settings->HardModeBGM.LoadSynchronous();

	case EVFGameDifficulty::VeryHard:
		return Settings->VeryHardModeBGM.LoadSynchronous();

	default:
		return nullptr;
	}
}

UVFGameInstance* UVFFunctionLibrary::GetVFGameInstance(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			return World->GetGameInstance<UVFGameInstance>();
		}
	}

	return nullptr;
}

void UVFFunctionLibrary::ToggleInputMode(const UObject* WorldContextObject, EVFInputMode InInputMode)
{
	APlayerController* PlayerController = nullptr;

	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			PlayerController = World->GetFirstPlayerController();
		}
	}

	if (!PlayerController)
	{
		return;
	}

	FInputModeGameOnly GameOnlyMode;
	FInputModeUIOnly UIOnlyMode;

	switch (InInputMode)
	{
	case EVFInputMode::GameOnly:
        
		PlayerController->SetInputMode(GameOnlyMode);
		PlayerController->bShowMouseCursor = false;

		break;

	case EVFInputMode::UIOnly:

		PlayerController->SetInputMode(UIOnlyMode);
		PlayerController->bShowMouseCursor = true;

		break;

	default:
		break;
	}
}

AActor* UVFFunctionLibrary::FindBestTargetHasGameplayTag(const UObject* WorldContextObject, 
		AActor* Instigator, 
		FGameplayTag TargetTag,
		const TArray<TEnumAsByte<EObjectTypeQuery>>& BoxTraceChannel,
		float BoxTraceDistance, 
		FVector TraceBoxSize,
		bool bDebugDraw)
{
	if (!Instigator || !WorldContextObject) return nullptr;

    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World) return nullptr;
	
    FVector Origin = Instigator->GetActorLocation();
	
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(Instigator);

    TArray<FHitResult> HitResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Instigator);

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		Instigator,
		Instigator->GetActorLocation(),
		Instigator->GetActorLocation() + Instigator->GetActorForwardVector() * BoxTraceDistance,
		TraceBoxSize / 2,
		Instigator->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceChannel,
		false,
		ActorsToIgnore,
		bDebugDraw ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResults,
		true
	);
	
    AActor* BestTarget = nullptr;
    float MinDistanceSq = FLT_MAX; // 紀錄最近距離的平方

    for (const FHitResult& Hit : HitResults)
    {
        AActor* HitActor = Hit.GetActor();
        if (!HitActor) continue;
    	
        if (!NativeDoesActorHaveTag(HitActor, TargetTag)) continue;
    	
        float DistSq = FVector::DistSquared(Origin, HitActor->GetActorLocation());
        if (DistSq < MinDistanceSq)
        {
            MinDistanceSq = DistSq;
            BestTarget = HitActor;
        }
    }
    return BestTarget;
}

FVector UVFFunctionLibrary::CalculateIdealExecutionLocation(const UObject* WorldContextObject, AActor* Attacker, AActor* Victim)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return FVector();

	FVector VictimLoc = Victim->GetActorLocation();
	FRotator VictimRot = Victim->GetActorRotation();

	float VictimRadius = 0.0f;
	float VictimHalfHeight = 0.0f;
	if (ACharacter* VicChar = Cast<ACharacter>(Victim))
	{
		VicChar->GetCapsuleComponent()->GetScaledCapsuleSize(VictimRadius, VictimHalfHeight);
	}

	float ExecutionDistance = 80.0f;

	FVector TargetOffset = VictimRot.Vector() * (VictimRadius + ExecutionDistance);

	return VictimLoc + TargetOffset;
}

FSlateBrush UVFFunctionLibrary::GetIconFromInputAction(const UObject* WorldContextObject, const UInputAction* InputAction, ECommonInputType InputType)
{
	FSlateBrush FoundBrush; // 預設空圖示

	// 安全檢查
	if (!WorldContextObject || !InputAction)
	{
		return FoundBrush;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PC) return FoundBrush;

	ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
	if (!LocalPlayer) return FoundBrush;

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!EnhancedInputSubsystem) return FoundBrush;

	TArray<FKey> BoundKeys = EnhancedInputSubsystem->QueryKeysMappedToAction(InputAction);
	if (BoundKeys.IsEmpty()) return FoundBrush;

	FKey TargetKey = BoundKeys[0];
	for (const FKey& Key : BoundKeys)
	{
		if (InputType == ECommonInputType::Gamepad && Key.IsGamepadKey())
		{
			TargetKey = Key;
			break;
		}
		else if (InputType == ECommonInputType::MouseAndKeyboard && !Key.IsGamepadKey())
		{
			TargetKey = Key;
			break;
		}
	}

	UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer);
	if (!CommonInputSubsystem) return FoundBrush;

	UCommonInputPlatformSettings::Get()->TryGetInputBrush(
		FoundBrush,
		TargetKey,
		InputType,
		CommonInputSubsystem->GetCurrentGamepadName()
	);

	return FoundBrush;
}

ECommonInputType UVFFunctionLibrary::GetCurrentInputType(const UObject* WorldContextObject)
{
	ECommonInputType DefaultInput = ECommonInputType::MouseAndKeyboard;

	if (!WorldContextObject) return DefaultInput;

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer))
			{
				return CommonInputSubsystem->GetCurrentInputType();
			}
		}
	}

	return DefaultInput;
}

void UVFFunctionLibrary::OpenLevelBasedOnDifficulty(const UObject* WorldContextObject)
{
	UVFGameUserSettings* Settings = UVFGameUserSettings::Get();
	if (!Settings) return;

	EVFGameDifficulty CurrentDifficulty = Settings->GetCurrentGameDifficulty();

	FGameplayTag LevelTag;
	switch (CurrentDifficulty)
	{
	case EVFGameDifficulty::Easy:
		LevelTag = VFGameplayTags::GameData_Level_SurvivalGameModeMap_Easy;
		break;
	case EVFGameDifficulty::Normal:
		LevelTag = VFGameplayTags::GameData_Level_SurvivalGameModeMap_Normal;
		break;
	case EVFGameDifficulty::Hard:
		LevelTag = VFGameplayTags::GameData_Level_SurvivalGameModeMap_Hard;
		break;
	case EVFGameDifficulty::VeryHard:
		LevelTag = VFGameplayTags::GameData_Level_SurvivalGameModeMap_VeryHard;
		break;
	default:
		LevelTag = VFGameplayTags::GameData_Level_SurvivalGameModeMap_Normal;
		break;
	}

	if (UVFGameInstance* GameInstance = GetVFGameInstance(WorldContextObject))
	{
		TSoftObjectPtr<UWorld> LevelToOpen = GameInstance->GetGameLevelByTag(LevelTag);

		if (!LevelToOpen.IsNull())
		{
			FName LevelName = FName(*LevelToOpen.GetAssetName());

			UGameplayStatics::OpenLevel(WorldContextObject, LevelName);
			ToggleInputMode(WorldContextObject, EVFInputMode::GameOnly);
		}
	}
}

