// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GasCan.generated.h"

UCLASS()
class XENON_API AGasCan : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGasCan();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* CanMesh;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	class USoundBase* CollectedSound;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void Floating(const float InDeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float RotateSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float RotateDirectionPercentage;

	UFUNCTION(BlueprintCallable, Category = "Collected")
	void Collected();

	UFUNCTION(BlueprintCallable, Category = "Collected")
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
