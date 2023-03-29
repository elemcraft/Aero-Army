// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaticTile.generated.h"

UCLASS()
class XENON_API AStaticTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStaticTile();

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
	class UStaticMeshComponent* TileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TileDisplacement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Tile")
	TArray<class UMaterialInterface*> TileMaterial;

	UPROPERTY(BlueprintReadWrite, Category = "Static Tile")
	class UMaterialInstanceDynamic* DynamicMaterial;
};
