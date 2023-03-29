// Fill out your copyright notice in the Description page of Project Settings.


#include "AggressiveMegaEnemyBase.h"
#include "Components/TimelineComponent.h"

AAggressiveMegaEnemyBase::AAggressiveMegaEnemyBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MuzzleFront = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Front"));
	MuzzleFront->SetupAttachment(RootComponent);

	MuzzleBack = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Back"));
	MuzzleBack->SetupAttachment(RootComponent);

	// Setup timeline
	EntranceTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Entrance Timeline"));
	ExitTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Exit Timeline"));
}

void AAggressiveMegaEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	// Variables
	AttackTimes = 5;

	// Setup entrance timeline
	FOnTimelineVector EntranceUpdateDelegate;
	EntranceUpdateDelegate.BindDynamic(this, &AAggressiveMegaEnemyBase::EntranceUpdate);
	EntranceTimeline->AddInterpVector(EntranceCurve, EntranceUpdateDelegate);
	FOnTimelineEvent EntranceFinishDelegate;
	EntranceFinishDelegate.BindDynamic(this, &AAggressiveMegaEnemyBase::EntranceFinish);
	EntranceTimeline->SetTimelineFinishedFunc(EntranceFinishDelegate);

	// Setup exit timeline
	FOnTimelineVector ExitUpdateDelegate;
	ExitUpdateDelegate.BindDynamic(this, &AAggressiveMegaEnemyBase::ExitUpdate);
	ExitTimeline->AddInterpVector(ExitCurve, ExitUpdateDelegate);
	FOnTimelineEvent ExitFinishDelegate;
	ExitFinishDelegate.BindDynamic(this, &AAggressiveMegaEnemyBase::ExitFinish);
	ExitTimeline->SetTimelineFinishedFunc(ExitFinishDelegate);

	EntranceTimeline->PlayFromStart();
}

void AAggressiveMegaEnemyBase::EntranceUpdate(const FVector Vector)
{
	SetActorLocation(Vector);
}

void AAggressiveMegaEnemyBase::EntranceFinish()
{
	Attack();
}

void AAggressiveMegaEnemyBase::ExitUpdate(const FVector Vector)
{
	SetActorLocation(Vector);
}

void AAggressiveMegaEnemyBase::ExitFinish()
{
	Destroy();
}

void AAggressiveMegaEnemyBase::Attack()
{
	// Spawn projectile
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		for (int32 i = 0; i < 5; i++)
		{
			FTransform Transform = FTransform(MuzzleFront->GetRelativeRotation() + FRotator(0, 30 * (i + 1), 0), MuzzleFront->GetComponentLocation());
			World->SpawnActor<AActor>(ProjectileClass, Transform, SpawnParameter);

			Transform = FTransform(MuzzleBack->GetRelativeRotation() + FRotator(0, 30 * (i + 1), 0), MuzzleBack->GetComponentLocation());
			World->SpawnActor<AActor>(ProjectileClass, Transform, SpawnParameter);
		}
	}

	AttackTimes -= 1;

	if (AttackTimes > 0)
	{
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AAggressiveMegaEnemyBase::Attack, 1.5f, false);
	}
	else
	{
		ExitTimeline->PlayFromStart();
	}
}
