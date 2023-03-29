// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FormationEnemyBase.h"
#include "SineAsteroidEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API ASineAsteroidEnemyBase : public AFormationEnemyBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	FVector SourcePoint;

	UPROPERTY(BlueprintReadOnly)
	float Frequency;

	UPROPERTY(BlueprintReadOnly)
	float Amplitude;

	UFUNCTION(BlueprintCallable)
	void SineMovement();
};
