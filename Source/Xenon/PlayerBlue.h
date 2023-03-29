// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerPawnBase.h"
#include "PlayerBlue.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API APlayerBlue : public APlayerPawnBase
{
	GENERATED_BODY()
	
public:
	virtual void CastSkill() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	class UParticleSystem* ElectricArcParticle;

	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	float ElectricDamage;

	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	float ElectricRadius;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void ApplyElectricDamage(UPARAM(ref) TArray<class AEnemyBase*> Enemies);

	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	TArray<class UParticleSystemComponent*> ElectricArc;

	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	int32 ElectricTimes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	class UParticleSystem* ElectricBurstParticle;
};
