// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDestroyer.h"
#include "EnemyBase.h"

void AEnemyDestroyer::BeginPlay()
{
	Super::BeginPlay();

	OnActorEndOverlap.AddDynamic(this, &AEnemyDestroyer::DestroyEnemy);
}

void AEnemyDestroyer::DestroyEnemy(AActor* OverlappedActor, AActor* OtherActor)
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);
	if (Enemy)
	{
		Enemy->Destroy();
		return;
	}
}
