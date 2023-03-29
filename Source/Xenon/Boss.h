// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Boss.generated.h"

UCLASS()
class XENON_API ABoss : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* BossMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* CollisionMesh;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	class ULevelHUD* HUDRef;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool bDead;

	UPROPERTY(BlueprintReadWrite, Category = "Health")
	float CurrentHealth;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void ReceiveDamage(const float Damage);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	UFUNCTION(BlueprintCallable, Category = "Death")
	void BossDie();

	UPROPERTY(EditAnywhere, Category = "Death")
	class USoundBase* BossDeathExplosion;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Damage Animation")
	TArray<class UMaterialInterface*> BossMaterialInterface;

	UPROPERTY(BlueprintReadWrite, Category = "Damage Animation")
	TArray<class UMaterialInstanceDynamic*> BossMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Animation")
	class UTimelineComponent* DamageAnimationTimeline;

	UPROPERTY(EditAnywhere, Category = "Damage Animation")
	class UCurveFloat* DamageAnimationCurve;

	UFUNCTION(BlueprintCallable, Category = "Damage Animation")
	void DamageAnimationUpdate(const float Value);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Entrance")
	class UTimelineComponent* EntranceTimeline;

	UPROPERTY(EditAnywhere, Category = "Entrance")
	class UCurveFloat* EntranceCurve;

	UFUNCTION(BlueprintCallable, Category = "Entrance")
	void EntranceUpdate(const float Value);

	UFUNCTION(BlueprintCallable, Category = "Entrance")
	void EntranceFinish();

	UPROPERTY(BlueprintReadOnly, Category = "Entrance")
	float ForwardSpeed;

public:
	UPROPERTY(BlueprintReadOnly, Category = "State Machine")
	bool bEntranceFinished;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void OverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	TArray<bool> AttackType;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	int32 CurrentAttackType;

protected:
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack();

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle AttackDelay;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle AttackDuration;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle AttackInterval;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Big Laser")
	class AActor* BigLaser;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Big Laser")
	TSubclassOf<class AActor> LaserActor;

	UFUNCTION(BlueprintCallable, Category = "Big Laser")
	void BigLaserEnable(const bool bEnable);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Small Jet")
	int32 SmallJetNum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Small Jet")
	TSubclassOf<class AActor> SmallJet;

	UFUNCTION(BlueprintCallable, Category = "Small Jet")
	void SmallJetEnable(const bool bEnable);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Mid Rotator")
	int32 RotatorAttackTimes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mid Rotator")
	TSubclassOf<class AActor> RotatorProjectile;

	UFUNCTION(BlueprintCallable, Category = "Mid Rotator")
	void MidRotatorEnable(const bool bEnable);
};
