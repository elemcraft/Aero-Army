// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuActor.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values

AMainMenuActor::AMainMenuActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = SceneRoot;

	SpaceshipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spaceship Mesh"));
	SpaceshipMesh->bVisibleInSceneCaptureOnly = true;
	SpaceshipMesh->SetupAttachment(RootComponent);

	ImageCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Image Capture"));
	ImageCapture->SetupAttachment(RootComponent);
	ImageCapture->SetRelativeLocation(FVector(200.0f, 200.0f, 200.0f));
	ImageCapture->SetRelativeRotation(FRotator(-40.000000f, -135.000000f, 0.000000f));
	ImageCapture->FOVAngle = 50.0f;

	RotateSpeed = 10.0f;
}

// Called when the game starts or when spawned
void AMainMenuActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Only capture actor component
	ImageCapture->ShowOnlyActorComponents(this);
}

// Called every frame
void AMainMenuActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotating SpaceshipMesh in main menu
	SpaceshipMesh->AddRelativeRotation(FRotator(0, DeltaTime * RotateSpeed, 0));
}

