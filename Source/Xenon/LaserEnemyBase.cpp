// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserEnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerPawnBase.h"
#include "LaserBeam.h"
#include "Components/AudioComponent.h"
#include "Components/TimelineComponent.h"


// Sets default values
ALaserEnemyBase::ALaserEnemyBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup components
	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(RootComponent);

	EntranceTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Entrance Timeline"));
	ExitTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Exit Timeline"));
}

// Called when the game starts or when spawned
void ALaserEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Variables
	AttackTimes = 0;

	// Setup entrance timeline
	FOnTimelineFloat EntranceDelegate;
	EntranceDelegate.BindDynamic(this, &ALaserEnemyBase::Entrance);
	EntranceTimeline->AddInterpFloat(EntranceCurve, EntranceDelegate);

	FOnTimelineEvent EntranceFinishDelegate;
	EntranceFinishDelegate.BindDynamic(this, &ALaserEnemyBase::Attack);
	EntranceTimeline->SetTimelineFinishedFunc(EntranceFinishDelegate);

	// Setup exit timeline
	FOnTimelineFloat ExitDelegate;
	ExitDelegate.BindDynamic(this, &ALaserEnemyBase::Exit);
	ExitTimeline->AddInterpFloat(ExitCurve, ExitDelegate);

	EntranceTimeline->PlayFromStart();
}

// Called every frame
void ALaserEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALaserEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALaserEnemyBase::Entrance(const float Value)
{
	FVector Location = GetActorLocation();
	Location.X = Value;
	SetActorLocation(Location);
}

void ALaserEnemyBase::Exit(const float Value)
{
	FVector Location = GetActorLocation();
	Location.X = Value;
	SetActorLocation(Location);
}

void ALaserEnemyBase::Attack()
{
	if (!bDead)
	{
		AttackTimes += 1;

		// Spawn Laser Beam
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		UWorld* World = GetWorld();
		LaserBeam = Cast<ALaserBeam>(World->SpawnActor<AActor>(LaserActor, Muzzle->GetComponentTransform(), SpawnParameter));

		// Shoot laser sound effect
		UAudioComponent* SoundEffect = UGameplayStatics::CreateSound2D(GetWorld(), ShootLaserSound);
		if (SoundEffect)
		{
			SoundEffect->Play();
		}

		// Set timer to destroy laser beam
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ALaserEnemyBase::RestoreEnergy);
		GetWorldTimerManager().SetTimer(RestoreTimer, TimerDelegate, 3.0f, false);
	}
}

void ALaserEnemyBase::RestoreEnergy()
{
	if (LaserBeam->ApplyDamageTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(LaserBeam->ApplyDamageTimer);
	}
	LaserBeam->Destroy();

	if (AttackTimes < 3)
	{
		// Set timer to spawn laser beam
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ALaserEnemyBase::Attack);
		GetWorldTimerManager().SetTimer(AttackTimer, TimerDelegate, 1.0f, false);
		return;
	}
	else
	{
		// Exit
		ExitTimeline->PlayFromStart();

		return;
	}
}

void ALaserEnemyBase::EnemyDie()
{
	Super::EnemyDie();

	// Destroy Timeline
	if (EntranceTimeline)
	{
		EntranceTimeline->DestroyComponent();
	}
	if (ExitTimeline)
	{
		ExitTimeline->DestroyComponent();
	}

	// Destroy Timer (AttackTimer, RestoreTimer)
	if (AttackTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
	}
	if (RestoreTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(RestoreTimer);
	}

	// Destroy laser beam if exist
	if (LaserBeam)
	{
		LaserBeam->Destroy();
	}
}