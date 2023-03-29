// Fill out your copyright notice in the Description page of Project Settings.


#include "SniperEnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerPawnBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/TimelineComponent.h"

#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASniperEnemyBase::ASniperEnemyBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(RootComponent);

	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel Mesh"));
	BarrelMesh->CastShadow = false;
	BarrelMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BarrelMesh->SetupAttachment(EnemyMesh);

	RecoilTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RecoilTimeline"));
}

// Called when the game starts or when spawned
void ASniperEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	// Get enemy material for death animation
	const int32 EnemyMaterialNum = EnemyMaterial.Num();
	TArray<UMaterialInterface*> BarrelMaterialInterface;
	BarrelMesh->GetUsedMaterials(BarrelMaterialInterface);
	for (int i = 0; i < BarrelMaterialInterface.Num() - 1; i++)
	{
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BarrelMaterialInterface[i], this);
		BarrelMesh->SetMaterial(i, DynamicMaterial);
		EnemyMaterial.Add(DynamicMaterial);
	}
	BarrelMaterialInterface.Empty();

	// Variables
	bAiming = false;
	ForwardSpeed = 65.0f;
	RotateSpeed = 100.0f;
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	EnemyMesh->OnComponentBeginOverlap.AddDynamic(this, &ASniperEnemyBase::Activate);

	// Setup recoil timeline
	FOnTimelineVector UpdateDelegate;
	UpdateDelegate.BindDynamic(this, &ASniperEnemyBase::RecoilAnimationUpdate);
	RecoilTimeline->AddInterpVector(RecoilCurve, UpdateDelegate);
}

// Called every frame
void ASniperEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveForward(DeltaTime);

	if (bAiming)
	{
		Aim(DeltaTime);
	}
}

// Called to bind functionality to input
void ASniperEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASniperEnemyBase::EnemyDie()
{
	Super::EnemyDie();

	if (AttackIntervalTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(AttackIntervalTimer);
	}
}

void ASniperEnemyBase::RecoilAnimationUpdate(const FVector Location)
{
	if (BarrelMesh)
	{
		BarrelMesh->SetRelativeLocation(Location);
	}
}

void ASniperEnemyBase::MoveForward(const float InDeltaTime)
{
	if (!bDead)
	{
		FVector Location = GetActorLocation();
		Location.X -= InDeltaTime * ForwardSpeed;
		SetActorLocation(Location);
	}
}

void ASniperEnemyBase::Attack(UParticleSystemComponent* PSystem)
{
	// Spawn projectile
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		World->SpawnActor<AActor>(ProjectileClass, Muzzle->GetComponentTransform(), SpawnParameter);
	}

	// Attack sound effect
	UAudioComponent* SoundEffect = UGameplayStatics::CreateSound2D(GetWorld(), AttackSound);
	if (SoundEffect)
	{
		SoundEffect->Play();
	}

	// Recoil animation 
	RecoilTimeline->PlayFromStart();

	// Timer
	GetWorldTimerManager().SetTimer(AttackIntervalTimer, this, &ASniperEnemyBase::SetTargetRotation, FMath::FRandRange(1.0f, 1.5f), false);
}

void ASniperEnemyBase::SetTargetRotation()
{
	const FVector PlayerLocation = PlayerPawn->GetActorLocation();
	TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerLocation);
	bAiming = true;
}

void ASniperEnemyBase::Aim(const float InDeltaTime)
{
	const bool bAimAtPlayer = FMath::Abs(GetActorRotation().Yaw - TargetRotation.Yaw) < 0.1f;
	if (bAimAtPlayer)
	{
		bAiming = false;

		// Spawn aiming particle effect
		UParticleSystemComponent* Particle = UGameplayStatics::SpawnEmitterAttached(AimingParticle, Muzzle, NAME_None, FVector(ForceInit), FRotator::ZeroRotator, FVector(0.12f), EAttachLocation::SnapToTarget);
		Particle->OnSystemFinished.AddDynamic(this, &ASniperEnemyBase::Attack);
	}
	else
	{
		FRotator Rotation = FMath::RInterpConstantTo(GetActorRotation(), TargetRotation, InDeltaTime, RotateSpeed);
		Rotation.Yaw = FMath::Clamp(Rotation.Yaw, -179.999f, 179.999f);   // Rotation.Yaw = 180.0f leads to error
		SetActorRotation(Rotation);
	}
}

void ASniperEnemyBase::Activate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Start attacking when sniper overlapped with BlockingVolumeTop
	if (OtherActor->Tags.IsValidIndex(1))
	{
		if (OtherActor->Tags[1] == FName("0"))
		{
			SetTargetRotation();
		}
	}
}
