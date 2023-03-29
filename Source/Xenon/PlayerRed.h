// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerPawnBase.h"
#include "PlayerRed.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API APlayerRed : public APlayerPawnBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	virtual void CastSkill() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	class UParticleSystem* HealBurstParticle;
	
	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	float RegenerateHealth;

	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	float GeneratedHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	float RegeneratePercentage;
};
