// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "LevelCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API ALevelCameraActor : public ACameraActor
{
	GENERATED_BODY()
	
private:
	ALevelCameraActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetCameraFieldOfView();

	UFUNCTION(BlueprintCallable)
	void SetCameraAspectRatio();
};
