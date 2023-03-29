// Fill out your copyright notice in the Description page of Project Settings.


#include "SineAsteroidEnemyBase.h"

void ASineAsteroidEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	// Variable
	SourcePoint = GetActorLocation();
	Amplitude = 35.0f;
	Frequency = 40.0f;
}

void ASineAsteroidEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SineMovement();
}

void ASineAsteroidEnemyBase::SineMovement()
{
	FVector Location = GetActorLocation();
	// Location.Y = SourcePoint.Y + FMath::Sin((Location.X) / Frequency) * Amplitude;
	Location.Y = SourcePoint.Y + (FMath::Sin((Location.X) / Frequency) - FMath::Sin((SourcePoint.X) / Frequency)) * Amplitude;
	SetActorLocation(Location);
}