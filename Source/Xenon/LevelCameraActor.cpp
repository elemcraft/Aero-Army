// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelCameraActor.h"
#include "Camera/CameraComponent.h"

ALevelCameraActor::ALevelCameraActor()
{
	SetCameraFieldOfView();
	SetCameraAspectRatio();
}

void ALevelCameraActor::BeginPlay()
{
	Super::BeginPlay();

}

void ALevelCameraActor::SetCameraFieldOfView()
{
	const float Radian = FMath::Atan(200.0f / 250.0f);
	const float Angle = 2 * (Radian / PI * 180.0f);
	GetCameraComponent()->SetFieldOfView(Angle);
}

void ALevelCameraActor::SetCameraAspectRatio()
{
	GetCameraComponent()->SetAspectRatio(9.0f / 16.0f);
}
