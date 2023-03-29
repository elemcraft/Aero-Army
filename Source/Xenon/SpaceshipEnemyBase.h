// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpaceshipEnemyBase.generated.h"

UCLASS()
class XENON_API ASpaceshipEnemyBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpaceshipEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Test
	UPROPERTY(BlueprintReadWrite)
	bool bDead;

	UPROPERTY(BlueprintReadWrite)
	class UMaterialInstanceDynamic* DyMat;

	UPROPERTY(BlueprintReadWrite)
	float Appearance;

	UPROPERTY(BlueprintReadWrite)
	float DisappearSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* EnemyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* Muzzle;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float ForwardSpeed;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(const float InDeltaTime);

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	FTimerHandle AttackTimer;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<class AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float MaxHealth;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void ReceiveDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void EnemyDie();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collide")
	float CollideDamage;

	UPROPERTY(BlueprintReadWrite, Category = "Collide")
	FTimerHandle OverlapTimer;

	UFUNCTION(BlueprintCallable, Category = "Collide")
	void ApplyCollideDamage();

	UFUNCTION(BlueprintCallable, Category = "Collide")
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Collide")
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward")
	TSubclassOf<class AActor> GasCanClass;

	UFUNCTION(BlueprintCallable, Category = "Reward")
	void SpawnGasCan();
};
