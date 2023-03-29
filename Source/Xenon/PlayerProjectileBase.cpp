// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyBase.h"
#include "PlayerPawnBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "Boss.h"

// Sets default values
APlayerProjectileBase::APlayerProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Setup components
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetRelativeScale3D(FVector(0.05));
	ProjectileMesh->CastShadow = false;
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 350.0f;
	ProjectileMovement->MaxSpeed = 350.0f;
	ProjectileMovement->ProjectileGravityScale = 0;

	ExplodeParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("Explode Particle"));
}

// Called when the game starts or when spawned
void APlayerProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &APlayerProjectileBase::HitEnemy);
	ProjectileMesh->OnComponentEndOverlap.AddDynamic(this, &APlayerProjectileBase::EndOverlap);
}

// Called every frame
void APlayerProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerProjectileBase::HitEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);
	if (Enemy)
	{
		const float ProjectileDamage = Cast<APlayerPawnBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->AttackValue;
		Enemy->ReceiveDamage(ProjectileDamage);

		// Explode particle
		UParticleSystemComponent* Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticle, GetActorLocation());
		Destroy();

		return;
	}

	ABoss* Boss = Cast<ABoss>(OtherActor);
	if (Boss)
	{
		const float ProjectileDamage = Cast<APlayerPawnBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->AttackValue;
		Boss->ReceiveDamage(ProjectileDamage);

		// Explode particle
		UParticleSystemComponent* Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticle, GetActorLocation());
		Destroy();

		return;
	}
}

void APlayerProjectileBase::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Tags.IsValidIndex(0))
	{
		if (OtherActor->Tags[0] == FName("LevelEdge"))
		{
			Destroy();
		}
	}
}
