// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "Components/TimelineComponent.h"
#include "LevelGameModeBase.h"
#include "LevelHUD.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerPawnBase.h"
#include "CustomSaveGame.h"

// Sets default values
ABoss::ABoss()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);

	BossMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Boss Mesh"));
	BossMesh->SetCastShadow(false);
	BossMesh->SetupAttachment(RootComponent);

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collision Mesh"));
	CollisionMesh->SetCastShadow(false);
	CollisionMesh->SetVisibility(false);
	CollisionMesh->SetCollisionProfileName(TEXT("Enemy"));
	CollisionMesh->SetupAttachment(RootComponent);

	EntranceTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Entrance Timeline"));
	DamageAnimationTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Damage Animation Timeline"));
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	// Variables
	bDead = false;
	bEntranceFinished = false;
	CurrentAttackType = -1;
	AttackType.Init(false, 3);

	// Gameplay Variable
	auto AdjustBasedOnDifficulty = [&](void) -> void
	{
		UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
		const float Difficulty = 1 + 0.25 * SaveGame->LevelIndex;

		MaxHealth *= Difficulty;
		CurrentHealth = MaxHealth;
	};
	AdjustBasedOnDifficulty();

	// Get Level HUD
	ALevelGameModeBase* GameMode = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	HUDRef = GameMode->HUDRef;

	// Get enemy material for damage/death animation
	BossMesh->GetUsedMaterials(BossMaterialInterface);
	for (int32 i = 0; i < BossMaterialInterface.Num(); i++)
	{
		if (i == 4 || i == 6 || i == 8)
		{
			BossMaterial.Add(UMaterialInstanceDynamic::Create(BossMaterialInterface[i], this));
		}
	}
	BossMaterialInterface.Empty();

	for (int32 i = 0; i < BossMaterial.Num(); i++)
	{
		BossMesh->SetMaterial(4 + i * 2, BossMaterial[i]);
	}

	// Damage animation timeline
	FOnTimelineFloat UpdateDelegate;
	UpdateDelegate.BindDynamic(this, &ABoss::DamageAnimationUpdate);
	DamageAnimationTimeline->AddInterpFloat(DamageAnimationCurve, UpdateDelegate);

	// Collision
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &ABoss::OverlapPlayer);

	// Entrance
	FOnTimelineFloat EntranceUpdateDelegate;
	EntranceUpdateDelegate.BindDynamic(this, &ABoss::EntranceUpdate);
	EntranceTimeline->AddInterpFloat(EntranceCurve, EntranceUpdateDelegate);
	FOnTimelineEvent EntranceFinishDelegate;
	EntranceFinishDelegate.BindDynamic(this, &ABoss::EntranceFinish);
	EntranceTimeline->SetTimelineFinishedFunc(EntranceFinishDelegate);

	EntranceTimeline->PlayFromStart();
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoss::BossDie()
{
	bDead = true;

	// Disable player take damage 
	APlayerPawnBase* Player = Cast<APlayerPawnBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Player->bReceiveDamage = false;
	
	// Clear Timer
	GetWorldTimerManager().ClearTimer(AttackDelay);
	GetWorldTimerManager().ClearTimer(AttackDuration);
	GetWorldTimerManager().ClearTimer(AttackInterval);

	// Destroy laser beam if necessary
	if (AttackType[0])
	{
		if (BigLaser)
		{
			BigLaser->Destroy();
		}
	}

	// Death effect
	HUDRef->PlayBossDeathAnimation();
	HUDRef->PlaySound(BossDeathExplosion);

	// Mission Success
	FTimerHandle MissionSuccessDelay;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(Player, &APlayerPawnBase::MissionSuccess);
	GetWorldTimerManager().SetTimer(MissionSuccessDelay, TimerDelegate, 4.5f, false);

	// Destroy Boss
	Destroy();
}

void ABoss::ReceiveDamage(const float Damage)
{
	if (!bDead)
	{
		CurrentHealth -= Damage;
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);

		// Update boss health bar
		HUDRef->UpdateGameProgressBar(CurrentHealth / MaxHealth);

		if (CurrentHealth == 0.0f)
		{
			BossDie();
			return;
		}
		else
		{
			// Play Receive Damage Animation
			DamageAnimationTimeline->PlayFromStart();
			return;
		}
	}
}

void ABoss::DamageAnimationUpdate(const float Value)
{
	for (UMaterialInstanceDynamic* Material : BossMaterial)
	{
		if (Material)
		{
			Material->SetScalarParameterValue(FName("DamageAppearance"), Value);
		}
	}
}

void ABoss::EntranceUpdate(const float Value)
{
	FVector Location = GetActorLocation();
	Location.X = Value;
	SetActorLocation(Location);
}

void ABoss::EntranceFinish()
{
	bEntranceFinished = true;

	FTimerHandle EntranceToIdleDelay;
	FTimerDelegate EnterIdle;
	EnterIdle.BindLambda([&]
	{
			Attack();

			// Start taking damage ( Enable collision )
			CollisionMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

			// HUD game progress bar animation
			HUDRef->PlayBossHealthBarAnimation();
	});

	GetWorldTimerManager().SetTimer(EntranceToIdleDelay, EnterIdle, 1.5f, false);
}

void ABoss::OverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerPawnBase* Player = Cast<APlayerPawnBase>(OtherActor);
	if (Player)
	{
		Player->ReceiveDamage(Player->CurrentHealth);
	}
}

void ABoss::Attack()
{
	const int32 Type = FMath::RandRange(0, AttackType.Num() - 1);
	// Avoid same attack type back-to-back
	if (Type == CurrentAttackType)
	{
		CurrentAttackType += 1;
		CurrentAttackType %= AttackType.Num();
	}
	else
	{
		CurrentAttackType = Type;
	}
	AttackType[CurrentAttackType] = true;
	
	if (CurrentAttackType == 0)
	{
		// Transform Duration = 23/24(frames) * 0.7(PlayRate) = 1.37(seconds)
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ABoss::BigLaserEnable, true);
		GetWorldTimerManager().SetTimer(AttackDelay, TimerDelegate, 1.37f, false);
		return;
	}

	if (CurrentAttackType == 1)
	{
		SmallJetNum = 8;

		// Transform Duration = 23/24(frames) * 0.7(PlayRate) = 1.37(seconds)
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ABoss::SmallJetEnable, true);
		GetWorldTimerManager().SetTimer(AttackDelay, TimerDelegate, 0.5f, true, 1.37f);
		return;
	}

	if(CurrentAttackType == 2)
	{
		RotatorAttackTimes = 40;

		// Transform Duration = 23/24(frames) * 0.7(PlayRate) = 1.37(seconds)
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ABoss::MidRotatorEnable, true);
		GetWorldTimerManager().SetTimer(AttackDelay, TimerDelegate, 0.6f, true, 1.37f);
		return;
	}
}

void ABoss::BigLaserEnable(const bool bEnable)
{
	if (bEnable)
	{
		// Spawn laser beam
		FTransform Transform = BossMesh->GetSocketTransform(FName("BodyLaserSocket"));
		Transform.SetScale3D(FVector::OneVector);
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		UWorld* World = GetWorld();
		BigLaser = World->SpawnActor<AActor>(LaserActor, Transform, SpawnParameter);
		BigLaser->AttachToComponent(BossMesh, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false), FName("BodyLaserSocket"));

		// Termination condition
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ABoss::BigLaserEnable, false);
		GetWorldTimerManager().SetTimer(AttackDuration, TimerDelegate, 5.0f, false);
	
		return;
	}
	else
	{
		BigLaser->Destroy();
		AttackType[0] = false;

		// Transform Duration = 23/24(frames) * 0.7(PlayRate) = 1.37(seconds)
		GetWorldTimerManager().SetTimer(AttackInterval, this, &ABoss::Attack, 2.0f, false);
	}
}

void ABoss::SmallJetEnable(const bool bEnable)
{
	if (bEnable)
	{
		// Spawn SineAsteroidEnemy
		FTransform Transform = BossMesh->GetSocketTransform(FName("BodyLaserSocket"));
		Transform.SetScale3D(FVector(0.7f));
		Transform.SetLocation(Transform.GetLocation() + FVector(-25, 0, 0));
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		UWorld* World = GetWorld();
		World->SpawnActor<AActor>(SmallJet, Transform, SpawnParameter);

		// Termination condition
		SmallJetNum -= 1;
		if (SmallJetNum == 0)
		{
			if (AttackDelay.IsValid())
			{
				GetWorldTimerManager().ClearTimer(AttackDelay);
			}
			SmallJetEnable(false);
		}

		return;
	}
	else
	{
		AttackType[1] = false;

		// Transform Duration = 23/24(frames) * 0.7(PlayRate) = 1.37(seconds)
		GetWorldTimerManager().SetTimer(AttackInterval, this, &ABoss::Attack, 2.0f, false);
	}
}

void ABoss::MidRotatorEnable(const bool bEnable)
{
	if (bEnable)
	{
		const float Rotation = [&](const float RotateMin, const float RotateMax, const int32 RotateTimes)
		{
			const int32 TotalAttackTimes = 40;
			const float Module = (RotatorAttackTimes - 1) % (TotalAttackTimes / RotateTimes);  // 0~9
			const int32 Quotient = (RotatorAttackTimes - 1) / (TotalAttackTimes / RotateTimes);  // 0~2
			const float Ratio = (Quotient % 2 == 0) ? (Module / (TotalAttackTimes / RotateTimes)) : (((TotalAttackTimes / RotateTimes - 1) - Module) / (TotalAttackTimes / RotateTimes));
			const float OutRotation = RotateMin + (RotateMax - RotateMin) * Ratio;
			return OutRotation;
		}(-35, 35, 4);

		// Spawn rotator projectile
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Left rotator
		FTransform Transform = BossMesh->GetSocketTransform(FName("LeftBodyRotatorSocket"));
		Transform.SetScale3D(FVector::OneVector);
		Transform.SetRotation((Transform.GetRotation().Rotator() + FRotator(0, Rotation, 0)).Quaternion());
		World->SpawnActor<AActor>(RotatorProjectile, Transform, SpawnParameter);

		// Right rotator
		Transform = BossMesh->GetSocketTransform(FName("RightBodyRotatorSocket"));
		Transform.SetScale3D(FVector::OneVector);
		Transform.SetRotation((Transform.GetRotation().Rotator() + FRotator(0, Rotation, 0)).Quaternion());
		World->SpawnActor<AActor>(RotatorProjectile, Transform, SpawnParameter);

		// Termination condition
		RotatorAttackTimes -= 1;
		if (RotatorAttackTimes == 0)
		{
			if (AttackDelay.IsValid())
			{
				GetWorldTimerManager().ClearTimer(AttackDelay);
			}
			MidRotatorEnable(false);
		}

		return;
	}
	else
	{
		AttackType[2] = false;

		// Transform Duration = 23/24(frames) * 0.7(PlayRate) = 1.37(seconds)
		GetWorldTimerManager().SetTimer(AttackInterval, this, &ABoss::Attack, 2.0f, false);
	}
}
