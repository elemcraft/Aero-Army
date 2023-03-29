// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LevelController.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API ALevelController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALevelController();

	virtual void BeginPlay() override;
};
