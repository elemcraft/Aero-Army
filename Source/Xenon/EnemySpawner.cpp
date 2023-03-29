// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "PlayerPawnBase.h"

#include "LevelGameModeBase.h"
#include "LevelHUD.h"

void AEnemySpawner::BeginPlay() 
{
	Super::BeginPlay();

	WaveCount = 11;
	WaveIndexRecord = -1;
	WaveType = 7;

	auto SetScreenEdge = [&](void) -> void
	{
		TArray<AActor*> Borders;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("LevelEdge"), Borders);
		ScreenEdge.Init(FVector::ZeroVector, Borders.Num());
		for (int i = 0; i < Borders.Num(); i++)
		{
			for (int k = 0; k < ScreenEdge.Num(); k++)
			{
				if (FString::FormatAsNumber(k) == Borders[i]->Tags[1].ToString())
				{
					ScreenEdge[k] = Borders[i]->GetActorLocation();
					break;
				}
			}
		}
		Borders.Empty();
	};
	SetScreenEdge();

	APlayerPawnBase* Player = Cast<APlayerPawnBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Player->EnableEnemySpawner.BindDynamic(this, &AEnemySpawner::Enable);
}

void AEnemySpawner::SpawnWave(int32 InWaveIndex)
{
	if (WaveCount > 0)
	{
		// Update game progress bar
		ALevelGameModeBase* GameMode = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); 
		GameMode->HUDRef->UpdateGameProgressBar((11 - WaveCount) / 11.0f); 
	}
	else
	{
		// Fill game progress bar
		ALevelGameModeBase* GameMode = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		GameMode->HUDRef->UpdateGameProgressBar(1);
		
		/*
		// Mission Success
		FTimerHandle MissionSuccessDelay;
		APlayerPawnBase* Player = Cast<APlayerPawnBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(Player, &APlayerPawnBase::MissionSuccess);
		GetWorldTimerManager().SetTimer(MissionSuccessDelay, TimerDelegate, 2.0f, false);
		*/
		SpawnBoss();
		return;
	}
	WaveCount -= 1;

	// Avoid spawn same type of enemy back to back
	if (InWaveIndex == WaveIndexRecord)
	{
		InWaveIndex += 1;
		InWaveIndex %= WaveType;
	}
	WaveIndexRecord = InWaveIndex; 

	// Next spawn delegate
	FTimerDelegate SpawnDelegate;
	SpawnDelegate.BindUObject(this, &AEnemySpawner::SpawnWave, FMath::RandRange(0, WaveType - 1));

	if (InWaveIndex == 0)
	{
		SpawnTurretWave(FMath::RandBool());
		GetWorldTimerManager().SetTimer(SpawnWaveTimer, SpawnDelegate, 12.0f, false);
		return;
	}
	
	if (InWaveIndex == 1)
	{
		SpawnFormationWave(FMath::RandRange(0, 3));
		GetWorldTimerManager().SetTimer(SpawnWaveTimer, SpawnDelegate, 5.0f, false);
		return;
	}

	if (InWaveIndex == 2)
	{
		SpawnLaserWave();
		GetWorldTimerManager().SetTimer(SpawnWaveTimer, SpawnDelegate, 14.0f, false);
		return;
	}

	if (InWaveIndex == 3)
	{
		SpawnSniperWave(FMath::RandBool());
		GetWorldTimerManager().SetTimer(SpawnWaveTimer, SpawnDelegate, 16.0f, false);
		return;
	}

	if (InWaveIndex == 4)
	{
		SpawnMegaWave(FMath::RandBool());
		GetWorldTimerManager().SetTimer(SpawnWaveTimer, SpawnDelegate, 13.0f, false);
		return;
	}

	if (InWaveIndex == 5)
	{
		SpawnLaserTurretWave(FMath::RandBool());
		GetWorldTimerManager().SetTimer(SpawnWaveTimer, SpawnDelegate, 15.0f, false);
		return;
	}

	if (InWaveIndex == 6)
	{
		SpawnSineAsteroidWave();
		GetWorldTimerManager().SetTimer(SpawnWaveTimer, SpawnDelegate, 6.0f, false);
		return;
	}
}

void AEnemySpawner::SpawnTurretWave(const bool Hardcore)
{
	FActorSpawnParameters SpawnParameter;
	SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (Hardcore)
	{
		// 6 Enemies
		for (int i = 0; i < 3; i++)
		{
			float LocationX = ScreenEdge[0].X + (ScreenEdge[1].X - ScreenEdge[0].X) * ((float(i) + 1.0f) / 4.0f);
			FVector Location = FVector(LocationX, 250, 200);
			FTransform EnemyTransform = FTransform(Location);
			UWorld* World = GetWorld();
			if (World)
			{
				World->SpawnActor<AActor>(EnemyPawn["TurretRight"], EnemyTransform, SpawnParameter);
			}
			
			Location.Y *= -1;
			EnemyTransform.SetLocation(Location);
			if (World)
			{
				World->SpawnActor<AActor>(EnemyPawn["TurretLeft"], EnemyTransform, SpawnParameter);
			}
		}
		return;
	}
	else
	{
		// 4 Enemies
		for (int i = 0; i < 2; i++)
		{
			float LocationX = ScreenEdge[0].X + (ScreenEdge[1].X - ScreenEdge[0].X) * ((float(i) + 1.0f) / 3.0f);
			FVector Location = FVector(LocationX, 250, 200);
			FTransform EnemyTransform = FTransform(Location);
			UWorld* World = GetWorld();
			if (World)
			{
				World->SpawnActor<AActor>(EnemyPawn["TurretRight"], EnemyTransform, SpawnParameter);
			}
			
			Location.Y *= -1;
			EnemyTransform.SetLocation(Location);
			if (World)
			{
				World->SpawnActor<AActor>(EnemyPawn["TurretLeft"], EnemyTransform, SpawnParameter);
			}
		}
		return;
	}
}

void AEnemySpawner::SpawnFormationWave(const int32 FormationIndex)
{
	FActorSpawnParameters SpawnParameter;
	SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (FormationIndex == 0)
	{
		// Triangle formation
		for (int Row = 0; Row < 3; Row++)
		{
			for (int Col = Row; Col < 5 - Row; Col++)
			{
				float LocationY = ScreenEdge[2].Y + (ScreenEdge[3].Y - ScreenEdge[2].Y) * ((float(Col) + 1.0f) / 6.0f);
				FVector Location = FVector(ScreenEdge[0].X + 60 * (3 - Row), LocationY, 200);
				FTransform EnemyTransform = FTransform(FRotator(0, 180, 0), Location);
				UWorld* World = GetWorld();
				if (World)
				{
					World->SpawnActor<AActor>(EnemyPawn["Formation"], EnemyTransform, SpawnParameter);
				}
			}
		}
		return;
	}
	else if (FormationIndex == 1)
	{
		// Parallel formation
		for (int Row = 0; Row < 5; Row++)
		{
			for (int Col = 0; Col < 6; Col++)
			{
				if (Col == 2 || Col == 3)
				{
					continue;
				}
				float LocationY = ScreenEdge[2].Y + (ScreenEdge[3].Y - ScreenEdge[2].Y) * ((float(Col) + 1.0f) / 7.0f);
				FVector Location = FVector(ScreenEdge[0].X + 60 * (Row + 1), LocationY, 200);
				FTransform EnemyTransform = FTransform(FRotator(0, 180, 0), Location);
				UWorld* World = GetWorld();
				if (World)
				{
					World->SpawnActor<AActor>(EnemyPawn["Formation"], EnemyTransform, SpawnParameter);
				}
				
			}
		}
		return;
	}
	else if (FormationIndex == 2)
	{
		// X formation
		for (int Row = 0; Row < 5; Row++)
		{
			int Col = Row;	
			float LocationY = ScreenEdge[2].Y + (ScreenEdge[3].Y - ScreenEdge[2].Y) * ((float(Col) + 1.0f) / 6.0f);
			FVector Location = FVector(ScreenEdge[0].X + 60 * (5 - Row), LocationY, 200);
			FTransform EnemyTransform = FTransform(FRotator(0, 180, 0), Location);
			UWorld* World = GetWorld();
			if (World)
			{
				World->SpawnActor<AActor>(EnemyPawn["Formation"], EnemyTransform, SpawnParameter);
			}

			Col = 4 - Row;
			if (Col == Row)
			{
				continue;
			}
			LocationY = ScreenEdge[2].Y + (ScreenEdge[3].Y - ScreenEdge[2].Y) * ((float(Col) + 1.0f) / 6.0f);
			Location = FVector(ScreenEdge[0].X + 60 * (5 - Row), LocationY, 200);
			EnemyTransform = FTransform(FRotator(0, 180, 0), Location);
			if (World)
			{
				World->SpawnActor<AActor>(EnemyPawn["Formation"], EnemyTransform, SpawnParameter);
			}
		}
		return;
	}
	else if (FormationIndex == 3)
	{
		// Upper left
		// arctan(9/16) = 29.36 degrees, 180 - 29.36 = 150.64
		FRotator Rotation = FRotator(0, 150.64f, 0);
		UWorld* World = GetWorld();
		for (int32 i = 0; i < 6; i++)
		{
			FVector Location = FVector(ScreenEdge[0].X + 16 * 5 * (i + 1), ScreenEdge[3].Y - 9 * 5 * (i + 1), 200);
			FTransform EnemyTransform = FTransform(Rotation, Location);
			World->SpawnActor<AActor>(EnemyPawn["FormationFast"], EnemyTransform, SpawnParameter);

			EnemyTransform.SetLocation(Location + FVector(-80, 0, 0));
			World->SpawnActor<AActor>(EnemyPawn["FormationFast"], EnemyTransform, SpawnParameter);
			
			EnemyTransform.SetLocation(Location + FVector(0, 45, 0));
			World->SpawnActor<AActor>(EnemyPawn["FormationFast"], EnemyTransform, SpawnParameter);
		}

		// Upper right
		Rotation *= -1;
		for (int32 i = 0; i < 6; i++)
		{
			FVector Location = FVector(ScreenEdge[0].X + 16 * 5 * (i + 1), ScreenEdge[2].Y + 9 * 5 * (i + 1), 200);
			FTransform EnemyTransform = FTransform(Rotation, Location);
			World->SpawnActor<AActor>(EnemyPawn["FormationFast"], EnemyTransform, SpawnParameter);

			EnemyTransform.SetLocation(Location + FVector(-80, 0, 0));
			World->SpawnActor<AActor>(EnemyPawn["FormationFast"], EnemyTransform, SpawnParameter);
			
			EnemyTransform.SetLocation(Location + FVector(0, -45, 0));
			World->SpawnActor<AActor>(EnemyPawn["FormationFast"], EnemyTransform, SpawnParameter);
		}
	}
}

void AEnemySpawner::SpawnLaserWave()
{
	FActorSpawnParameters SpawnParameter;
	SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int i = 0; i < 3; i++)
	{
		float LocationY = ScreenEdge[2].Y + (ScreenEdge[3].Y - ScreenEdge[2].Y) * ((float(i) + 1.0f) / 4.0f);
		FVector Location = FVector(ScreenEdge[0].X + 60, LocationY, 200);
		FTransform EnemyTransform = FTransform(FRotator(0, 180, 0), Location);

		UWorld* World = GetWorld();
		if (World)
		{
			World->SpawnActor<AActor>(EnemyPawn["Laser"], EnemyTransform, SpawnParameter);
		}
	}
}

void AEnemySpawner::SpawnMegaWave(const bool bLeft)
{
	FActorSpawnParameters SpawnParameter;
	SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (bLeft)
	{
		// Mega enemy on the left side
		UWorld* World = GetWorld();
		if (World)
		{
			const FVector Location = FVector(ScreenEdge[0].X + 250, ScreenEdge[3].Y + 50, 200);
			const FTransform EnemyTransform = FTransform(FRotator(0, 180, 0), Location);
			World->SpawnActor<AActor>(EnemyPawn["AggressiveMegaLeft"], EnemyTransform, SpawnParameter);
		}

		// Formation enemy on the right side
		if (World)
		{
			for (int32 Row = 0; Row < 4; Row++)
			{
				for (int32 Col = 0; Col < 3; Col++)
				{
					const FVector Location = FVector(ScreenEdge[0].X + 80 * (Row + 1), ScreenEdge[2].Y - 60 * (Col + 1), 200);
					FTransform EnemyTransform = FTransform(FRotator(0, 180, 0), Location);
					World->SpawnActor<AActor>(EnemyPawn["Formation"], EnemyTransform, SpawnParameter);
				}
			}
		}
	}
	else
	{
		// Mega enemy on the right side
		UWorld* World = GetWorld();
		if (World)
		{
			const FVector Location = FVector(ScreenEdge[0].X + 250, ScreenEdge[2].Y - 50, 200);
			const FTransform EnemyTransform = FTransform(FRotator(0, 180, 0), Location);
			World->SpawnActor<AActor>(EnemyPawn["AggressiveMegaRight"], EnemyTransform, SpawnParameter);
		}

		// Formation enemy on the left side
		if (World)
		{
			for (int32 Row = 0; Row < 4; Row++)
			{
				for (int32 Col = 0; Col < 3; Col++)
				{
					const FVector Location = FVector(ScreenEdge[0].X + 80 * (Row + 1), ScreenEdge[3].Y + 60 * (Col + 1), 200);
					FTransform EnemyTransform = FTransform(FRotator(0, 180, 0), Location);
					World->SpawnActor<AActor>(EnemyPawn["Formation"], EnemyTransform, SpawnParameter);
				}
			}
		}
	}
	
}

void AEnemySpawner::SpawnSniperWave(const bool Hardcore)
{
	FActorSpawnParameters SpawnParameter;
	SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const float LocationYLeft = ScreenEdge[3].Y + 50.0f;
	const float LocationYRight = ScreenEdge[2].Y - 50.0f;
	
	// Spawn snipers
	for (int Row = 0; Row < 3; Row++)
	{
		const FVector LocationLeft = FVector(ScreenEdge[0].X + 150 * (Row + 0.5f), LocationYLeft, 200);
		const FVector LocationRight = FVector(ScreenEdge[0].X + 150 * (Row + 0.5f), LocationYRight, 200);
		const FTransform EnemyTransformLeft = FTransform(FRotator(0, 180, 0), LocationLeft);
		const FTransform EnemyTransformRight = FTransform(FRotator(0, 180, 0), LocationRight);

		UWorld* World = GetWorld();
		if (World)
		{
			World->SpawnActor<AActor>(EnemyPawn["Sniper"], EnemyTransformLeft, SpawnParameter);
			World->SpawnActor<AActor>(EnemyPawn["Sniper"], EnemyTransformRight, SpawnParameter);
		}
	}

	if (Hardcore)
	{
		// Mega enemy in the middle
		UWorld* World = GetWorld();
		if (World)
		{
			const FVector Location = FVector(ScreenEdge[0].X + 250, 0, 200);
			const FTransform EnemyTransform = FTransform(FRotator(0, 180, 0), Location);
			World->SpawnActor<AActor>(EnemyPawn["Mega"], EnemyTransform, SpawnParameter);
		}
	}
	else
	{
		// Formation in the middle
		const float Middle = (ScreenEdge[2].Y + ScreenEdge[3].Y) / 2;
		for (int Row = 0; Row < 10; Row++)
		{
			if (Row == 4 || Row == 5)
			{
				continue;
			}

			for (int Col = 0; Col < 2; Col++)
			{
				FVector Location = FVector(ScreenEdge[0].X + 60 * (Row + 1), Middle - 25.0f, 200);
				Location.Y = (Col < 1) ? (Middle - 25.0f) : (Middle + 25.0f);
				FTransform EnemyTransform = FTransform(FRotator(0, 180, 0), Location);
				UWorld* World = GetWorld();
				if (World)
				{
					World->SpawnActor<AActor>(EnemyPawn["Formation"], EnemyTransform, SpawnParameter);
				}
			}
		}
	}
}

void AEnemySpawner::SpawnLaserTurretWave(const bool bLeft)
{
	FActorSpawnParameters SpawnParameter;
	SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (bLeft)
	{
		FVector Location = FVector(90.0f, -250, 200);
		FTransform EnemyTransform = FTransform(Location);
		UWorld* World = GetWorld();
		if (World)
		{
			World->SpawnActor<AActor>(EnemyPawn["LaserTurretLeft"], EnemyTransform, SpawnParameter);

			Location.X *= -1;
			EnemyTransform.SetLocation(Location);
			World->SpawnActor<AActor>(EnemyPawn["LaserTurretLeft"], EnemyTransform, SpawnParameter);
		}
	}
	else
	{
		FVector Location = FVector(90.0f, 250, 200);
		FTransform EnemyTransform = FTransform(Location);
		UWorld* World = GetWorld();
		if (World)
		{
			World->SpawnActor<AActor>(EnemyPawn["LaserTurretRight"], EnemyTransform, SpawnParameter);

			Location.X *= -1;
			EnemyTransform.SetLocation(Location);
			World->SpawnActor<AActor>(EnemyPawn["LaserTurretRight"], EnemyTransform, SpawnParameter);
		}
	}
}

void AEnemySpawner::SpawnSineAsteroidWave()
{
	FActorSpawnParameters SpawnParameter;
	SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int32 Row = 0; Row < 6; Row++)
	{
		for (int32 Col = 0; Col < 3; Col++)
		{
			FVector Location = FVector(ScreenEdge[0].X + 60 * (Row + 1), 80 * (Col - 1), 200);
			FTransform EnemyTransform = FTransform(FRotator(0, 180, 0), Location);
			UWorld* World = GetWorld();
			World->SpawnActor<AActor>(EnemyPawn["SineAsteroid"], EnemyTransform, SpawnParameter);
		}
	}
}

void AEnemySpawner::SpawnBoss()
{
	FActorSpawnParameters SpawnParameter;
	SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UWorld* World = GetWorld();
	FVector Location = FVector(300, 0, 200);
	FTransform EnemyTransform = FTransform(FRotator(0, 180, 0), Location);
	World->SpawnActor<AActor>(EnemyPawn["Boss"], EnemyTransform, SpawnParameter);
}

void AEnemySpawner::Enable(bool bEnable)
{
	if (bEnable)
	{
		// Start spawning enemy
		SpawnWave(FMath::RandRange(0, WaveType - 1));
		return;
	}
	else
	{
		GetWorldTimerManager().ClearTimer(SpawnWaveTimer);
		return;
	}
}
