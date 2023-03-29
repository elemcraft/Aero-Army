// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerPawnBase.h"
#include "PlayerGreen.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API APlayerGreen : public APlayerPawnBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	APlayerGreen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void CastSkill() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* VisualShieldMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* ShieldMesh;

	UPROPERTY(BlueprintReadWrite)
	float ShieldTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	class UParticleSystem* ShieldBurstParticle;
};
