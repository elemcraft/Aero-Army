// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainMenuActor.generated.h"

UCLASS()
class XENON_API AMainMenuActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainMenuActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* SpaceshipMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneCaptureComponent2D* ImageCapture;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RotateSpeed;
};
