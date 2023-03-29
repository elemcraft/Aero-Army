// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "EnemySpawner.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class XENON_API AEnemySpawner : public AVolume
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	int32 WaveCount;

	UPROPERTY(BlueprintReadOnly)
	int32 WaveType;

	UPROPERTY(BlueprintReadOnly)
	int32 WaveIndexRecord;

	UPROPERTY(BlueprintReadOnly, meta = (Tooltip = "Top, Bottom, Right, Left"))
	TArray<FVector> ScreenEdge;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle SpawnWaveTimer;

	UPROPERTY(EditAnywhere, Category = "Enemy Pawn")
	TMap<FName, TSubclassOf<class AActor>> EnemyPawn;

	UFUNCTION(BlueprintCallable)
	void SpawnWave(int32 InWaveIndex);

	UFUNCTION(BlueprintCallable)
	void SpawnTurretWave(const bool Hardcore);

	UFUNCTION(BlueprintCallable)
	void SpawnFormationWave(const int32 FormationIndex);

	UFUNCTION(BlueprintCallable)
	void SpawnLaserWave();

	UFUNCTION(BlueprintCallable)
	void SpawnMegaWave(const bool bLeft);

	UFUNCTION(BlueprintCallable)
	void SpawnSniperWave(const bool Hardcore);

	UFUNCTION(BlueprintCallable)
	void SpawnLaserTurretWave(const bool bLeft);

	UFUNCTION(BlueprintCallable)
	void SpawnSineAsteroidWave();

	UFUNCTION(BlueprintCallable)
	void SpawnBoss();

public:
	UFUNCTION(BlueprintCallable)
	void Enable(bool bEnable);
};
