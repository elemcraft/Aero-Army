// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "LaserTurretEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API ALaserTurretEnemyBase : public AEnemyBase
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	ALaserTurretEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void EnemyDie() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* MuzzleOne;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* MuzzleTwo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* MuzzleThree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* MuzzleFour;

	UPROPERTY(BlueprintReadOnly)
	TArray<class USceneComponent*> Muzzle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Entrance")
	class UTimelineComponent* EntranceTimeline;

	UPROPERTY(EditAnywhere, Category = "Entrance")
	class UCurveFloat* EntranceCurve;

	UFUNCTION(BlueprintCallable, Category = "Entrance")
	void Entrance(const float Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Entrance")
	class UTimelineComponent* ExitTimeline;

	UPROPERTY(EditAnywhere, Category = "Entrance")
	class UCurveFloat* ExitCurve;

	UFUNCTION(BlueprintCallable, Category = "Exit")
	void Exit(const float Value);

	UFUNCTION(BlueprintCallable)
	void Spin(const float InDeltaTime);

	UPROPERTY(BlueprintReadOnly)
	float RotateSpeed;

protected:
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Laser")
	TSubclassOf<class AActor> LaserActor;

	UPROPERTY(BlueprintReadWrite)
	TArray<class ALaserBeam*> LaserBeam;

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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spin")
	class UTimelineComponent* SpinTimeline;

	UPROPERTY(EditAnywhere, Category = "Spin")
	class UCurveFloat* SpinCurve;

	UFUNCTION(BlueprintCallable, Category = "Spin")
	void SpinUpdate(const float Value);

	UPROPERTY(BlueprintReadOnly, Category = "Spin")
	FTimerHandle SpinDelay;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "1: clockwise, -1: Counterclockwise"), Category = "Spin")
	int32 SpinDirection;
};
