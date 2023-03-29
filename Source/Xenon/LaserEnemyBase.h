// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "LaserEnemyBase.generated.h"

UCLASS()
class XENON_API ALaserEnemyBase : public AEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALaserEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* Muzzle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Entrance")
	class UTimelineComponent* EntranceTimeline;

	UPROPERTY(EditAnywhere, Category = "Entrance")
	class UCurveFloat* EntranceCurve;

	UFUNCTION(BlueprintCallable, Category = "Entrance")
	void Entrance(const float Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Exit")
	class UTimelineComponent* ExitTimeline;

	UPROPERTY(EditAnywhere, Category = "Exit")
	class UCurveFloat* ExitCurve;

	UFUNCTION(BlueprintCallable, Category = "Exit")
	void Exit(const float Value);

	virtual void EnemyDie() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Laser")
	TSubclassOf<class AActor> LaserActor;

	UPROPERTY(BlueprintReadWrite)
	class ALaserBeam* LaserBeam;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void RestoreEnergy();

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	FTimerHandle AttackTimer;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	FTimerHandle RestoreTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	class USoundBase* ShootLaserSound;

	UPROPERTY(BlueprintReadOnly)
	int32 AttackTimes;
};
