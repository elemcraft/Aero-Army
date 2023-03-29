// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserTurretEnemyBase.h"
#include "Components/TimelineComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LaserBeam.h"

#include "Kismet/KismetMathLibrary.h"

ALaserTurretEnemyBase::ALaserTurretEnemyBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup components
	MuzzleOne = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle One"));
	MuzzleOne->SetupAttachment(RootComponent);
	
	MuzzleTwo = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Two"));
	MuzzleTwo->SetupAttachment(RootComponent);

	MuzzleThree = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Three"));
	MuzzleThree->SetupAttachment(RootComponent);

	MuzzleFour = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Four"));
	MuzzleFour->SetupAttachment(RootComponent);

	Muzzle.Add(MuzzleOne);
	Muzzle.Add(MuzzleTwo);
	Muzzle.Add(MuzzleThree);
	Muzzle.Add(MuzzleFour);

	EntranceTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Entrance Timeline"));
	SpinTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Spin Timeline"));
	ExitTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Exit Timeline"));
}

void ALaserTurretEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	// Variables
	// SpinDirection = 1;
	AttackTimes = 4;
	RotateSpeed = 20.0f;

	// Setup exit timeline
	FOnTimelineFloat ExitDelegate;
	ExitDelegate.BindDynamic(this, &ALaserTurretEnemyBase::Exit);
	ExitTimeline->AddInterpFloat(ExitCurve, ExitDelegate);
	FOnTimelineEvent ExitFinishDelegate;
	ExitFinishDelegate.BindDynamic(this, &ALaserTurretEnemyBase::EnemyDie);
	ExitTimeline->SetTimelineFinishedFunc(ExitFinishDelegate);

	// Setup spin timeline
	FOnTimelineFloat SpinDelegate;
	SpinDelegate.BindDynamic(this, &ALaserTurretEnemyBase::SpinUpdate);
	SpinTimeline->AddInterpFloat(SpinCurve, SpinDelegate);
	FOnTimelineEvent SpinFinishDelegate;
	SpinFinishDelegate.BindDynamic(this, &ALaserTurretEnemyBase::RestoreEnergy);
	SpinTimeline->SetTimelineFinishedFunc(SpinFinishDelegate);

	// Setup entrance timeline
	FOnTimelineFloat EntranceDelegate;
	EntranceDelegate.BindDynamic(this, &ALaserTurretEnemyBase::Entrance);
	EntranceTimeline->AddInterpFloat(EntranceCurve, EntranceDelegate);
	FOnTimelineEvent EntranceFinishDelegate;
	EntranceFinishDelegate.BindDynamic(this, &ALaserTurretEnemyBase::Attack);
	EntranceTimeline->SetTimelineFinishedFunc(EntranceFinishDelegate);

	EntranceTimeline->PlayFromStart();
}

void ALaserTurretEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaserTurretEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALaserTurretEnemyBase::Entrance(const float Value)
{
	FVector Location = GetActorLocation();
	Location.Y = Value;
	SetActorLocation(Location);
}

void ALaserTurretEnemyBase::Exit(const float Value)
{
	FVector Location = GetActorLocation();
	Location.Y = Value;
	SetActorLocation(Location);
}

void ALaserTurretEnemyBase::Attack()
{
	if (!bDead)
	{
		AttackTimes -= 1;

		// Spawn Laser Beam
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		UWorld* World = GetWorld();
		for (int32 i = 0; i < 4; i++)
		{
			LaserBeam.Add(World->SpawnActor<ALaserBeam>(LaserActor, Muzzle[i]->GetComponentTransform(), SpawnParameter));
			LaserBeam[i]->AttachToComponent(Muzzle[i], FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));	
		}

		// Shoot laser sound effect
		UAudioComponent* SoundEffect = UGameplayStatics::CreateSound2D(GetWorld(), ShootLaserSound);
		if (SoundEffect)
		{
			SoundEffect->Play();
		}

		// Spin
		GetWorldTimerManager().SetTimer(SpinDelay, SpinTimeline, &UTimelineComponent::PlayFromStart, 0.5f, false);
	}
}

void ALaserTurretEnemyBase::RestoreEnergy()
{
	SpinDirection *= -1;

	for (ALaserBeam* Laser : LaserBeam)
	{
		if (Laser->ApplyDamageTimer.IsValid())
		{
			GetWorldTimerManager().ClearTimer(Laser->ApplyDamageTimer);
		}
		Laser->Destroy();
	}
	LaserBeam.Empty();

	if (AttackTimes > 0)
	{
		// Set timer to spawn laser beam
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ALaserTurretEnemyBase::Attack);
		GetWorldTimerManager().SetTimer(AttackTimer, TimerDelegate, 1.0f, false);
		return;
	}
	else
	{
		ExitTimeline->PlayFromStart();
		return;
	}
}

void ALaserTurretEnemyBase::SpinUpdate(const float Value)
{
	if (!bDead)
	{
		FRotator Rotation = FRotator(0, SpinDirection * Value, 0);
		SetActorRotation(Rotation);

		for (int32 i = 0; i < 4; i++)
		{
			if (LaserBeam.IsValidIndex(i))
			{
				LaserBeam[i]->SourcePoint = Muzzle[i]->GetComponentLocation();
				LaserBeam[i]->ForwardDirection = LaserBeam[i]->GetActorForwardVector();
			}
		}
	}
}

void ALaserTurretEnemyBase::Spin(const float InDeltaTime)
{
	if (!bDead)
	{
		FRotator Rotation = FRotator(0, InDeltaTime * RotateSpeed, 0);
		AddActorWorldRotation(Rotation);

		for (int32 i = 0; i < 4; i++)
		{
			if (LaserBeam.IsValidIndex(i))
			{
				LaserBeam[i]->SourcePoint = Muzzle[i]->GetComponentLocation();
				LaserBeam[i]->ForwardDirection = LaserBeam[i]->GetActorForwardVector();
			}
		}
	}
}

void ALaserTurretEnemyBase::EnemyDie()
{
	Super::EnemyDie();

	// Destroy Timeline
	if (EntranceTimeline)
	{
		EntranceTimeline->DestroyComponent();
	}
	if (SpinTimeline)
	{
		SpinTimeline->DestroyComponent();
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
	if (SpinDelay.IsValid())
	{
		GetWorldTimerManager().ClearTimer(SpinDelay);
	}

	// Destroy laser beam if exist
	for (ALaserBeam* Laser : LaserBeam)
	{
		if (Laser)
		{
			Laser->Destroy();
		}
	}
}
