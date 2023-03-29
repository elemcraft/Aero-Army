// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MegaEnemyBase.h"
#include "AggressiveMegaEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API AAggressiveMegaEnemyBase : public AMegaEnemyBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	AAggressiveMegaEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Entrance")
	class UTimelineComponent* EntranceTimeline;

	UPROPERTY(EditAnywhere, Category = "Entrance")
	class UCurveVector* EntranceCurve;

	UFUNCTION(BlueprintCallable, Category = "Entrance")
	void EntranceUpdate(const FVector Vector);

	UFUNCTION(BlueprintCallable, Category = "Entrance")
	void EntranceFinish();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Exit")
	class UTimelineComponent* ExitTimeline;

	UPROPERTY(EditAnywhere, Category = "Exit")
	class UCurveVector* ExitCurve;

	UFUNCTION(BlueprintCallable, Category = "Exit")
	void ExitUpdate(const FVector Vector);

	UFUNCTION(BlueprintCallable, Category = "Exit")
	void ExitFinish();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* MuzzleFront;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* MuzzleBack;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")		
	int32 AttackTimes;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack();

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<class AActor> ProjectileClass;
};
