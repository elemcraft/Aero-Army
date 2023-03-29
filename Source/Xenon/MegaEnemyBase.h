// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "MegaEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API AMegaEnemyBase : public AEnemyBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	AMegaEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void ReceiveDamage(const float Damage) override;

	virtual void CollideWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bMoveForward;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float ForwardSpeed;

	UFUNCTION(BlueprintCallable, Category = "Movement")		
	void MoveForward(const float InDeltaTime);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	class USoundBase* HitSound;
};
