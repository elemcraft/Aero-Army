// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "SniperEnemyBase.generated.h"

UCLASS()
class XENON_API ASniperEnemyBase : public AEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASniperEnemyBase();

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
	class USceneComponent* Muzzle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* BarrelMesh;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float ForwardSpeed;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(const float InDeltaTime);

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float RotateSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	class USoundBase* AttackSound;

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	class AActor* PlayerPawn;

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	bool bAiming;

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	FRotator TargetRotation;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetTargetRotation();

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	FTimerHandle AttackIntervalTimer;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack(class UParticleSystemComponent* PSystem);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<class AActor> ProjectileClass;
	
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Aim(const float InDeltaTime);

protected:
	UFUNCTION(BlueprintCallable)
	void Activate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recoil")
	class UTimelineComponent* RecoilTimeline;

	UFUNCTION(BlueprintCallable, Category = "Recoil")
	void RecoilAnimationUpdate(const FVector Location);

	UPROPERTY(EditAnywhere, Category = "Recoil")
	class UCurveVector* RecoilCurve;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	class UParticleSystem* AimingParticle;
};
