// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticTile.h"
#include "CustomSaveGame.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AStaticTile::AStaticTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile Mesh"));
	TileMesh->SetupAttachment(RootComponent);

	TileSpeed = 0.1f;
}

// Called when the game starts or when spawned
void AStaticTile::BeginPlay()
{
	Super::BeginPlay();

	// Set tile material
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
	DynamicMaterial = UMaterialInstanceDynamic::Create(TileMaterial[SaveGame->ChapterIndex], this);
	TileMesh->SetMaterial(0, DynamicMaterial);

	TileDisplacement = 0.0f;
}

// Called every frame
void AStaticTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TileDisplacement += TileSpeed * DeltaTime;
	TileDisplacement = TileDisplacement - FMath::Floor(TileDisplacement);
	DynamicMaterial->SetScalarParameterValue(TEXT("Displacement"), TileDisplacement);
}

