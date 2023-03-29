// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "FormationEnemyBase.generated.h"

UCLASS()
class XENON_API AFormationEnemyBase : public AEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFormationEnemyBase();

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

	virtual void CollideWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector ForwardDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float ForwardSpeed;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(const float InDeltaTime);
};
