// Paradise NiseMono All Rights Reserved


#include "GameModes/VFSurvivalGameMode.h"

#include "NavigationSystem.h"
#include "VFDebugHelper.h"
#include "Characters/VFEnemyCharacter.h"
#include "Engine/AssetManager.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

void AVFSurvivalGameMode::OnLoadingScreenDeactivated_Implementation()
{
	SetCurrentSurvivalGameModeState(EVFSurvivalGameModeState::WaitSpawnNewWave);
}
void AVFSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentSurvivalGameModeState(EVFSurvivalGameModeState::Loading);

	if (UDataTable** FoundTable = DifficultyWaveSpawnerDataTables.Find(CurrentGameDifficulty))
	{
		CurrentActiveWaveDataTable = *FoundTable;
	}

	checkf(CurrentActiveWaveDataTable, TEXT("Forgot to assign a valid data table for current difficulty in survival game mode blueprint!"));

	TotalWavesToSpawn = CurrentActiveWaveDataTable->GetRowNames().Num();

	PreLoadNextWaveEnemies();
}

void AVFSurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentSurvivalGameModeState == EVFSurvivalGameModeState::WaitSpawnNewWave && CurrentSurvivalGameModeState != EVFSurvivalGameModeState::Loading)
	{
		TimePassedSinceStart += DeltaTime;

		if (TimePassedSinceStart >= SpawnNewWaveWaitTime)
		{
			TimePassedSinceStart = 0.f;

			SetCurrentSurvivalGameModeState(EVFSurvivalGameModeState::SpawningNewWave);
		}
	}

	if (CurrentSurvivalGameModeState == EVFSurvivalGameModeState::SpawningNewWave)
	{
		TimePassedSinceStart += DeltaTime;

		if (TimePassedSinceStart >= SpawnEnemiesDelayTime)
		{
			CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();

			TimePassedSinceStart = 0.f;

			SetCurrentSurvivalGameModeState(EVFSurvivalGameModeState::InProgress);
		}
	}

	if (CurrentSurvivalGameModeState == EVFSurvivalGameModeState::WaveCompleted)
	{
		TimePassedSinceStart += DeltaTime;

		if (TimePassedSinceStart >= WaveCompletedWaitTime)
		{
			TimePassedSinceStart = 0.f;

			CurrentWaveCount++;

			if (HasFinishedAllWaves())
			{
				SetCurrentSurvivalGameModeState(EVFSurvivalGameModeState::AllWavesDone);
			}
			else
			{
				SetCurrentSurvivalGameModeState(EVFSurvivalGameModeState::WaitSpawnNewWave);
				PreLoadNextWaveEnemies();
			}
		}
	}
}

void AVFSurvivalGameMode::SetCurrentSurvivalGameModeState(EVFSurvivalGameModeState InState)
{
	CurrentSurvivalGameModeState = InState;

	OnSurvivalGameModeStateChanged.Broadcast(CurrentSurvivalGameModeState);
}

bool AVFSurvivalGameMode::HasFinishedAllWaves() const
{
	return CurrentWaveCount > TotalWavesToSpawn;
}

void AVFSurvivalGameMode::PreLoadNextWaveEnemies()
{
	if (HasFinishedAllWaves())
	{
		return;
	}

	PreLoadedEnemyClassMap.Empty();

	for (const FVFEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if(SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;

		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			SpawnerInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda(
				[SpawnerInfo,this]()
				{
					if (UClass* LoadedEnemyClass = SpawnerInfo.SoftEnemyClassToSpawn.Get())
					{
						PreLoadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSpawn,LoadedEnemyClass);
					}
				}
			)
		);
	}
}

FVFEnemyWaveSpawnerTableRow* AVFSurvivalGameMode::GetCurrentWaveSpawnerTableRow() const
{
	const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));

	FVFEnemyWaveSpawnerTableRow* FoundRow = CurrentActiveWaveDataTable->FindRow<FVFEnemyWaveSpawnerTableRow>(RowName, FString());
	
	checkf(FoundRow,TEXT("Could not find a valid row under the name %s in the data table"),*RowName.ToString());

	return FoundRow;
}

int32 AVFSurvivalGameMode::TrySpawnWaveEnemies()
{
	if (TargetPointsArray.IsEmpty())
	{
		UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointsArray);
	}
	
	checkf(!TargetPointsArray.IsEmpty(), TEXT("No valid target point found in level: %s for spawning enemies"), *GetWorld()->GetName());

	uint32 EnemiesSpawnedThisTime = 0;

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (const FVFEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if(SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;

		const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawnCount, SpawnerInfo.MaxPerSpawnCount);

		UClass* LoadedEnemyClass = PreLoadedEnemyClassMap.FindChecked(SpawnerInfo.SoftEnemyClassToSpawn);

		for (int32 i = 0; i < NumToSpawn; i++)
		{
			int32 RandomTargetPointIndex = FMath::RandRange(0,TargetPointsArray.Num() - 1);
			FVector SpawnOrigin = TargetPointsArray[RandomTargetPointIndex]->GetActorLocation();
			FRotator SpawnRotation = TargetPointsArray[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();

			FVector RandomLocation;
			UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, SpawnOrigin, RandomLocation, 450.f);

			RandomLocation += FVector(0.f,0.f,150.f);

			AVFEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AVFEnemyCharacter>(LoadedEnemyClass, RandomLocation, SpawnRotation, SpawnParam);

			if (SpawnedEnemy)
			{
				EnemiesSpawnedThisTime++;
				TotalSpawnedEnemiesThisWaveCounter++;
				SpawnedEnemy->SetEnemyLevel(SpawnerInfo.EnemyLevel);
				SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);
			}
			else
			{
				bool bSpawnSuccess = false;
				int32 RetryCount = 0;
				
				while (!bSpawnSuccess && RetryCount < 10)
				{
					RandomTargetPointIndex = FMath::RandRange(0,TargetPointsArray.Num() - 1);
					SpawnOrigin = TargetPointsArray[RandomTargetPointIndex]->GetActorLocation();
					SpawnRotation = TargetPointsArray[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();
						
					UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, SpawnOrigin, RandomLocation, 450.f);
						
					AActor* NewEnemy = GetWorld()->SpawnActor<AVFEnemyCharacter>(LoadedEnemyClass, RandomLocation, SpawnRotation, SpawnParam);

					if (NewEnemy)
					{
						bSpawnSuccess = true;
					}
					else
					{
						RetryCount++;
					}
				}

				if (!bSpawnSuccess)
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to spawn enemy after 10 retries!"));
				}
			}
			
			if (!ShouldKeepSpawnEnemies())
			{
				return EnemiesSpawnedThisTime;
			}
		}
	}

	return EnemiesSpawnedThisTime;
	
}

bool AVFSurvivalGameMode::ShouldKeepSpawnEnemies() const
{
	return TotalSpawnedEnemiesThisWaveCounter < GetCurrentWaveSpawnerTableRow()->TotalEnemyToSpawnThisWave;
}

void AVFSurvivalGameMode::OnEnemyDestroyed(AActor* DestroyedActor)
{
	CurrentSpawnedEnemiesCounter--;

	//Debug::Print(FString::Printf(TEXT("CurrentSpawnedEnemiesCounter:%i, TotalSpawnedEnemiesThisWaveCounter:%i"),CurrentSpawnedEnemiesCounter,TotalSpawnedEnemiesThisWaveCounter));

	if (ShouldKeepSpawnEnemies())
	{
		CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
	}
	else if (CurrentSpawnedEnemiesCounter == 0)
	{
		TotalSpawnedEnemiesThisWaveCounter = 0;
		CurrentSpawnedEnemiesCounter = 0;

		SetCurrentSurvivalGameModeState(EVFSurvivalGameModeState::WaveCompleted);
	}
}

void AVFSurvivalGameMode::RegisterSpawnedEnemies(const TArray<AVFEnemyCharacter*>& InEnemiesToRegister)
{
	if (InEnemiesToRegister.IsEmpty()) return;

	for (AVFEnemyCharacter* SpawnedEnemy : InEnemiesToRegister)
	{
		if (SpawnedEnemy)
		{
			CurrentSpawnedEnemiesCounter++;

			SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this,&ThisClass::OnEnemyDestroyed);
		}
	}
}
