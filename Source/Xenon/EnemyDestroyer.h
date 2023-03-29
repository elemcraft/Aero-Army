// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "EnemyDestroyer.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API AEnemyDestroyer : public AVolume
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void DestroyEnemy(AActor* OverlappedActor, AActor* OtherActor);
};
