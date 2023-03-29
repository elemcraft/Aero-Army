// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyBase.generated.h"

UCLASS()
class XENON_API AEnemyBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyBase();

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
	class USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* EnemyMesh;

	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float MaxHealth;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void ReceiveDamage(const float Damage);

	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void EnemyDie();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collide")
	float CollideDamage;

	UPROPERTY(BlueprintReadWrite, Category = "Collide")
	FTimerHandle CollideTimer;
	
	FTimerDelegate CollideTimerDelegate;

	UFUNCTION(BlueprintCallable, Category = "Collide")
	virtual void CollideWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Collide")
	virtual void EndCollideWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	bool bDead;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death Animation")
	float AppearanceMax;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death Animation")
	float AppearanceMin;

	UPROPERTY(BlueprintReadOnly, Category = "Death Animation")
	float DisappearSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	TArray<class UMaterialInstanceDynamic*> EnemyMaterial;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Animation")
	class UTimelineComponent* DamageTimeline;

	UPROPERTY(EditAnywhere, Category = "Damage Animation")
	class UCurveFloat* DamageAnimationCurve;

	UFUNCTION(BlueprintCallable, Category = "Damage Animation")
	void DamageAnimationUpdate(const float Value);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward")
	TSubclassOf<class AActor> GasCanClass;

	UFUNCTION(BlueprintCallable, Category = "Reward")
	void SpawnGasCan();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward")
	float SpawnGasCanProbability;
};
