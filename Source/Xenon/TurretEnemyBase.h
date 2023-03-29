// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "TurretEnemyBase.generated.h"

UCLASS()
class XENON_API ATurretEnemyBase : public AEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATurretEnemyBase();

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
	TArray<class UStaticMeshComponent*> BarrelMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TArray<class UStaticMeshComponent*> SupportMesh;

	UPROPERTY(BlueprintReadWrite, Category = "Spin")
	float RotateSpeed;

	UFUNCTION(BlueprintCallable, Category = "Spin")
	void Spin(const float InDeltaTime);

	UPROPERTY(BlueprintReadOnly, Category = "Spin")
	bool bSpin;

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	FTimerHandle AttackTimer;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TArray<FVector> MuzzleLocationArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TArray<FRotator> MuzzleRotationArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<class AActor> ProjectileClass;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Entrance")
	class UTimelineComponent* EntranceTimeline;

	UPROPERTY(EditAnywhere, Category = "Entrance")
	class UCurveFloat* EntranceCurve;

	UFUNCTION(BlueprintCallable, Category = "Entrance")
	void Entrance(const float Value);

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Guns Up")
	class UTimelineComponent* GunsUpTimeline;

	UPROPERTY(EditAnywhere, Category = "Guns Up")
	class UCurveFloat* GunsUpCurve;

	UFUNCTION(BlueprintCallable, Category = "Guns Up")
	void GunsUp(const float Value);

protected:
	UFUNCTION(BlueprintCallable)
	void Activate();

	UPROPERTY(BlueprintReadOnly, Category = "Exit")
	FTimerHandle ExitTimer;

	UFUNCTION(BlueprintCallable, Category = "Exit")
	void Exit();
};
