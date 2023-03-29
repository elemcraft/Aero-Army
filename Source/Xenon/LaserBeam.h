// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserBeam.generated.h"

UCLASS()
class XENON_API ALaserBeam : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserBeam();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* BeamMesh;

	UPROPERTY(BlueprintReadOnly)
	float StretchSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LaserDamage;

	UPROPERTY(BlueprintReadWrite)
	bool bHitPlayer;

	UPROPERTY(BlueprintReadWrite)
	bool bStretching;
	
	UPROPERTY(BlueprintReadWrite)
	FVector SourcePoint;
	
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle ApplyDamageTimer;

protected:
	UFUNCTION(BlueprintCallable)
	void HitPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	/*
	UFUNCTION(BlueprintCallable)
	void OldElongateLaser(const float InDeltaTime);

	UFUNCTION(BlueprintCallable)
	void OldCutBeamLength();
	*/

public:
	UPROPERTY(BlueprintReadOnly)
	FVector ForwardDirection;
protected:
	UPROPERTY(BlueprintReadOnly)
	float BeamLength;

	UFUNCTION(BlueprintCallable)
	void CutBeamLength();

	UFUNCTION(BlueprintCallable)
	void ElongateLaser(const float InDeltaTime);
};
