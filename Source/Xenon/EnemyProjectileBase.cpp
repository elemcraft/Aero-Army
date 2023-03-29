// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PlayerPawnBase.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/BlockingVolume.h"

#include "CustomSaveGame.h"

// Sets default values
AEnemyProjectileBase::AEnemyProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetCollisionProfileName(TEXT("Projectile"));
	ProjectileMesh->SetRelativeScale3D(FVector(0.05));
	ProjectileMesh->CastShadow = false;
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void AEnemyProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectileBase::HitPlayer);
	ProjectileMesh->OnComponentEndOverlap.AddDynamic(this, &AEnemyProjectileBase::EndOverlap);
}

// Called every frame
void AEnemyProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyProjectileBase::HitPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Apply damage to player
	APlayerPawnBase* Player = Cast<APlayerPawnBase>(OtherActor);
	if (Player)
	{
		// Adjust enemy projectile damage based on level difficulty
		UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
		const float Difficulty = 1 + 0.25 * SaveGame->LevelIndex;

		Player->ReceiveDamage(ProjectileDamage * Difficulty);
		Destroy();
	}
}

void AEnemyProjectileBase::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABlockingVolume* Wall = Cast<ABlockingVolume>(OtherActor);
	if (Wall)
	{
		Destroy();
	}
}