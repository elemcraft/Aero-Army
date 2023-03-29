// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelController.h"
#include "PlayerPawnBase.h"

ALevelController::ALevelController()
{
	
}

void ALevelController::BeginPlay()
{
	Super::BeginPlay();

	// Disable virtual joystick in game
	ActivateTouchInterface(nullptr);
}