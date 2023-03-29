// Fill out your copyright notice in the Description page of Project Settings.


#include "FormationEnemyBase.h"
#include "PlayerPawnBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFormationEnemyBase::AFormationEnemyBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFormationEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Variables
	ForwardDirection = GetActorForwardVector();
}

// Called every frame
void AFormationEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveForward(DeltaTime);
}

// Called to bind functionality to input
void AFormationEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFormationEnemyBase::EnemyDie()
{
	Super::EnemyDie();

}

void AFormationEnemyBase::CollideWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerPawnBase* Player = Cast<APlayerPawnBase>(OtherActor);
	if (Player)
	{
		Player->ReceiveDamage(CollideDamage);
		EnemyDie();
		return;
	}
}

void AFormationEnemyBase::MoveForward(const float InDeltaTime)
{
	if (!bDead)
	{
		FVector Location = GetActorLocation();
		Location += InDeltaTime * ForwardSpeed * ForwardDirection;
		SetActorLocation(Location);
	}
}

