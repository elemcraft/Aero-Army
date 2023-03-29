// Fill out your copyright notice in the Description page of Project Settings.


#include "MegaEnemyBase.h"
#include "PlayerPawnBase.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

AMegaEnemyBase::AMegaEnemyBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMegaEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	// Variables
	ForwardSpeed = 65.0f;
}

void AMegaEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bMoveForward)
	{
		MoveForward(DeltaTime);
	}
}

void AMegaEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

void AMegaEnemyBase::ReceiveDamage(const float Damage)
{
	// Mage enemy doesn't take any damage

	// Sound Effect
	UAudioComponent* SoundEffect = UGameplayStatics::CreateSound2D(this, HitSound);
	if (SoundEffect)
	{
		SoundEffect->Play();
	}

	return;
}

void AMegaEnemyBase::CollideWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerPawnBase* Player = Cast<APlayerPawnBase>(OtherActor);
	if (Player)
	{
		Player->ReceiveDamage(Player->CurrentHealth);
	}
}

void AMegaEnemyBase::MoveForward(const float InDeltaTime)
{
	if (!bDead)
	{
		FVector Location = GetActorLocation();
		Location.X -= InDeltaTime * ForwardSpeed;
		SetActorLocation(Location);
	}
}
