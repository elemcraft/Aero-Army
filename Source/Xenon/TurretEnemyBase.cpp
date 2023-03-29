// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretEnemyBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerPawnBase.h"

#include "Components/TimelineComponent.h"

// Sets default values
ATurretEnemyBase::ATurretEnemyBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup components
	for (unsigned int i = 0; i < 4; i++)
	{
		const FString String = "BarrelMesh" + FString::FormatAsNumber(i);
		BarrelMesh.Add(CreateDefaultSubobject<UStaticMeshComponent>(FName(*String)));
		BarrelMesh[i]->CastShadow = false;
		BarrelMesh[i]->SetupAttachment(RootComponent);
	}

	for (unsigned int i = 0; i < 4; i++)
	{
		const FString String = "SupportMesh" + FString::FormatAsNumber(i);
		SupportMesh.Add(CreateDefaultSubobject<UStaticMeshComponent>(FName(*String)));
		SupportMesh[i]->CastShadow = false;
		SupportMesh[i]->SetupAttachment(RootComponent);
	}

	GunsUpTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("GunsUpTimeline"));
	EntranceTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Entrance Timeline"));

	MuzzleLocationArray.Add(FVector(33, 0, 3));
	MuzzleLocationArray.Add(FVector(0, 33, 3));
	MuzzleLocationArray.Add(FVector(-33, 0, 3));
	MuzzleLocationArray.Add(FVector(0, -33, 3));

	MuzzleRotationArray.Add(FRotator::ZeroRotator);
	MuzzleRotationArray.Add(FRotator(0, 90, 0));
	MuzzleRotationArray.Add(FRotator(0, 180, 0));
	MuzzleRotationArray.Add(FRotator(0, 270, 0));
}

// Called when the game starts or when spawned
void ATurretEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Variables
	RotateSpeed = 80.0f;
	bSpin = false;
	
	// Setup Guns Up Timeline 
	FOnTimelineFloat UpdateDelegate;
	UpdateDelegate.BindDynamic(this, &ATurretEnemyBase::GunsUp);
	GunsUpTimeline->AddInterpFloat(GunsUpCurve, UpdateDelegate);
		
	FOnTimelineEvent GunsUpFinishDelegate;
	GunsUpFinishDelegate.BindDynamic(this, &ATurretEnemyBase::Activate);
	GunsUpTimeline->SetTimelineFinishedFunc(GunsUpFinishDelegate);

	// Setup entrance timeline
	FOnTimelineFloat EntranceDelegate;
	EntranceDelegate.BindDynamic(this, &ATurretEnemyBase::Entrance);
	EntranceTimeline->AddInterpFloat(EntranceCurve, EntranceDelegate);

	FOnTimelineEvent EntranceFinishDelegate;
	EntranceFinishDelegate.BindDynamic(GunsUpTimeline, &UTimelineComponent::PlayFromStart);
	EntranceTimeline->SetTimelineFinishedFunc(EntranceFinishDelegate);

	EntranceTimeline->PlayFromStart();
}

// Called every frame
void ATurretEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSpin)
	{
		Spin(DeltaTime);
	}
}

// Called to bind functionality to input
void ATurretEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATurretEnemyBase::Entrance(const float Value)
{
	FVector Location = GetActorLocation();
	Location.Y = Value;
	SetActorLocation(Location);
}

void ATurretEnemyBase::Activate()
{
	bSpin = true;
	Attack();

	// Set exit timer
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ATurretEnemyBase::Exit);
	GetWorldTimerManager().SetTimer(ExitTimer, TimerDelegate, 10.0f, false);
}

void ATurretEnemyBase::Spin(const float InDeltaTime)
{
	if (!bDead)
	{
		FRotator Rot = GetActorRotation();
		Rot.Yaw += InDeltaTime * RotateSpeed;
		SetActorRotation(Rot);

		for (auto Mesh : SupportMesh)
		{
			if (Mesh)
			{
				Mesh->AddRelativeRotation(FRotator(0, -InDeltaTime * RotateSpeed, 0));
			}
		}
	}
}

void ATurretEnemyBase::Attack()
{
	if (!bDead)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParameter;
			SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			for (int i = 0; i < 4; i++)
			{
				const FVector WorldLocation = UKismetMathLibrary::TransformLocation(RootComponent->GetComponentTransform(), MuzzleLocationArray[i]);
				const FRotator WorldRotation = UKismetMathLibrary::TransformRotation(RootComponent->GetComponentTransform(), MuzzleRotationArray[i]);
				const FTransform Tranform = FTransform(WorldRotation, WorldLocation, FVector::OneVector);

				World->SpawnActor<AActor>(ProjectileClass, Tranform, SpawnParameter);
			}
		}

		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ATurretEnemyBase::Attack);
		GetWorldTimerManager().SetTimer(AttackTimer, TimerDelegate, 0.6f, false);
	}
}

void ATurretEnemyBase::GunsUp(const float Value)
{
	if (BarrelMesh[0])
	{
		BarrelMesh[0]->SetRelativeLocation(FVector(Value, 0.0f, 0.0f));
	}
	if (BarrelMesh[1])
	{
		BarrelMesh[1]->SetRelativeLocation(FVector(0.0f, Value, 0.0f));
	}
	if (BarrelMesh[2])
	{
		BarrelMesh[2]->SetRelativeLocation(FVector(-Value, 0.0f, 0.0f));
	}
	if (BarrelMesh[3])
	{
		BarrelMesh[3]->SetRelativeLocation(FVector(0.0f, -Value, 0.0f));
	}
}

void ATurretEnemyBase::Exit()
{
	if (AttackTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
	}

	bSpin = false;

	FOnTimelineEvent ExitFinishDelegate;
	ExitFinishDelegate.BindDynamic(this, &ATurretEnemyBase::EnemyDie);
	EntranceTimeline->SetTimelineFinishedFunc(ExitFinishDelegate);

	FOnTimelineEvent GunsDownFinishDelegate;
	GunsDownFinishDelegate.BindDynamic(EntranceTimeline, &UTimelineComponent::ReverseFromEnd);
	GunsUpTimeline->SetTimelineFinishedFunc(GunsDownFinishDelegate);

	GunsUpTimeline->ReverseFromEnd();
}

void ATurretEnemyBase::EnemyDie()
{
	Super::EnemyDie();

	if (AttackTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
	}

	if (ExitTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(ExitTimer);
	}

	if (GunsUpTimeline)
	{
		GunsUpTimeline->DestroyComponent();
	}
	if (EntranceTimeline)
	{
		EntranceTimeline->DestroyComponent();
	}

	for (UStaticMeshComponent* Mesh : BarrelMesh)
	{
		if (Mesh)
		{
			Mesh->DestroyComponent();
		}
	}

	for (UStaticMeshComponent* Mesh : SupportMesh)
	{
		if (Mesh)
		{
			Mesh->DestroyComponent();
		}
	}
}