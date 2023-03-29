// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawnBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"
#include "CustomSaveGame.h"
#include "PlayerProjectileBase.h"

#include "LevelGameModeBase.h"

#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "GameFramework/HUD.h"

#include "LevelHUD.h"

#include "Components/AudioComponent.h"


// Sets default values
APlayerPawnBase::APlayerPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; 

	// Setup components
	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Player Mesh"));
	PlayerMesh->SetCollisionProfileName(TEXT("Player"));
	PlayerMesh->CastShadow = false;
	SetRootComponent(PlayerMesh);

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual Mesh"));
	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VisualMesh->SetCollisionProfileName(FName("NoCollision"), false);
	VisualMesh->CastShadow = false;
	VisualMesh->SetupAttachment(RootComponent);

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(RootComponent);

	ExplodeParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("Explode Particle"));

	// Setup player animation timeline
	EntranceAnimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("EntranceAnimTimeline"));
	ExitAnimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ExitAnimTimeline"));

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerPawnBase::BeginPlay()
{
	Super::BeginPlay();

	// Variables
	bReceiveDamage = true;
	GasCanCount = 0;

	// Setup movement variable
	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Block player input before animation ends
	DisableInput(PC);

	// Setup Entrance Animation
	FOnTimelineFloat EntranceUpdateDelegate;
	EntranceUpdateDelegate.BindDynamic(this, &APlayerPawnBase::EntranceAnimationUpdate);
	EntranceAnimTimeline->AddInterpFloat(EntranceAnimCurve, EntranceUpdateDelegate);
	FOnTimelineEvent EntranceFinishDelegate;
	EntranceFinishDelegate.BindDynamic(this, &APlayerPawnBase::EntranceAnimationFinish);
	EntranceAnimTimeline->SetTimelineFinishedFunc(EntranceFinishDelegate);

	// Setup Exit Animation
	FOnTimelineFloat ExitUpdateDelegate;
	ExitUpdateDelegate.BindDynamic(this, &APlayerPawnBase::ExitAnimationUpdate);
	ExitAnimTimeline->AddInterpFloat(ExitAnimCurve, ExitUpdateDelegate);
	FOnTimelineEvent ExitFinishDelegate;
	ExitFinishDelegate.BindDynamic(this, &APlayerPawnBase::ExitAnimationFinish);
	ExitAnimTimeline->SetTimelineFinishedFunc(ExitFinishDelegate);

	// Set Mesh Visibility
	PlayerMesh->SetVisibility(false);
	VisualMesh->SetVisibility(false);
}

// Called every frame
void APlayerPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void APlayerPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &APlayerPawnBase::TouchStartFunction);
	PlayerInputComponent->BindTouch(IE_Repeat, this, &APlayerPawnBase::TouchRepeatFunction);
}

void APlayerPawnBase::PlayEntranceAnimation()
{
	PlayerMesh->SetVisibility(true);
	EntranceAnimTimeline->PlayFromStart();

	// Player enter sound effect
	UAudioComponent* SoundEffect = UGameplayStatics::CreateSound2D(GetWorld(), PlayerEnterSound);
	if (SoundEffect)
	{
		SoundEffect->Play();
	}
}

void APlayerPawnBase::EntranceAnimationUpdate(const float Value)
{
	const float AnimationDistance = 400.0f;
	const float LocationX = AnimationDistance * (Value - 1);
	SetActorLocation(FVector(LocationX, 0, 200));
	SetActorRotation(FRotator(0, 0, 3600 * Value));
}

void APlayerPawnBase::EntranceAnimationFinish()
{
	// Enable player input
	bool bPressed;
	PC->GetInputTouchState(ETouchIndex::Touch1, TouchStartPosition.X, TouchStartPosition.Y, bPressed);
	if (!bPressed)
	{
		TouchStartPosition = FVector2D::ZeroVector;
	}
	EnableInput(PC);

	// Start attacking
	GetWorldTimerManager().SetTimer(AttackTimer, this, &APlayerPawnBase::Attack, 1.0f / AttackSpeed, true);

	// Set Mesh Visibility
	PlayerMesh->SetVisibility(false);
	VisualMesh->SetVisibility(true);

	// Start spawn enemy
	EnableEnemySpawner.Execute(true);

	// Play background music
	ALevelGameModeBase* GameMode = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->PlayBackgroundMusic();
}

void APlayerPawnBase::ExitAnimationUpdate(const float Value)
{
	SetActorLocation(EndLocation + FVector(Value, 0, 0));
}

void APlayerPawnBase::ExitAnimationFinish()
{
	ALevelGameModeBase* GameMode = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	ULevelHUD* HUD = Cast<ULevelHUD>(GameMode->HUDRef);
	HUD->HUDMissionSuccess();

	// Clear Skill Timer
	if (SkillTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(SkillTimer);
	}
}

void APlayerPawnBase::Attack()
{
	UWorld* World = GetWorld();
	if (World)
	{
		// Spawn projectile
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		World->SpawnActor<AActor>(ProjectileClass, Muzzle->GetComponentTransform(), SpawnParameter);

		// Attack sound effect
		UAudioComponent* SoundEffect = UGameplayStatics::CreateSound2D(GetWorld(), PlayerAttackSound);
		if (SoundEffect)
		{
			SoundEffect->Play();
		}
	}
}

void APlayerPawnBase::ReceiveDamage(const float Damage)
{
	if (bReceiveDamage)
	{
		// Taking damage animation
		TakeDamageAnimation.Execute();

		// Receive damage sound effect
		UAudioComponent* SoundEffect = UGameplayStatics::CreateSound2D(GetWorld(), PlayerTakeDamageSound);
		if (SoundEffect)
		{
			SoundEffect->Play();
		}

		CurrentHealth -= Damage;
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);

		UpdateHealthBar.Broadcast(CurrentHealth / MaxHealth);

		if (CurrentHealth == 0)
		{
			PlayerDie();
		}
	}
}

void APlayerPawnBase::MissionSuccess()
{
	bReceiveDamage = false;

	// Disable spawn enemy
	EnableEnemySpawner.Execute(false);

	// Disable attack
	GetWorldTimerManager().ClearTimer(AttackTimer);

	// Disable player input movement
	DisableInput(PC);

	// Save reward and progress
	SaveRewardAndProgress(true);

	// Exit Animation
	EndLocation = GetActorLocation();
	ExitAnimTimeline->PlayFromStart();
	
	// Exit animation sound effect
	auto PlayerExitSoundEffect = [&](void) -> void
	{
		UAudioComponent* SoundEffect = UGameplayStatics::CreateSound2D(GetWorld(), PlayerLeaveSound);
		if (SoundEffect)
		{
			SoundEffect->Play();
		}
	};
	FTimerHandle SoundDelay;
	GetWorldTimerManager().SetTimer(SoundDelay, PlayerExitSoundEffect, 0.8f, false, 0.8f);
	
	// HUD Animation
	ALevelGameModeBase* GameMode = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	ULevelHUD* HUD = Cast<ULevelHUD>(GameMode->HUDRef);
	HUD->PlayAnimation(HUD->HUDAnimation, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.2f);
}

void APlayerPawnBase::SaveRewardAndProgress(const bool bMissionSuccess)
{
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));

	if (bMissionSuccess)
	{
		// Reward
		const float DifficultyMultiplier = (1 + 0.25f * SaveGame->LevelIndex);
		SaveGame->ItemData[0] += FMath::Floor(GasCanCount * DifficultyMultiplier);
		Reward.Add(GasCanCount);
		Reward.Add(DifficultyMultiplier * 100);
		Reward.Add(FMath::Floor(GasCanCount * DifficultyMultiplier));

		for (int32 i = 0; i < 4; i++)
		{
			const int32 RewardCount = (i == SaveGame->PlayerIndex + 1) ? (5 * (SaveGame->LevelIndex + 1)) : (3 * (SaveGame->LevelIndex + 1));
			SaveGame->ItemData[i] += RewardCount;
			Reward.Add(RewardCount);
		}

		// Progress
		if (SaveGame->LevelIndex + 1 > SaveGame->ChapterProgress[SaveGame->ChapterIndex])
		{
			SaveGame->ChapterProgress[SaveGame->ChapterIndex] = SaveGame->LevelIndex + 1;
		}
	}
	else
	{
		SaveGame->ItemData[0] += FMath::Floor(GasCanCount * 0.5f);
		Reward.Add(GasCanCount);
		Reward.Add(50); 
		Reward.Add(FMath::Floor(GasCanCount * 0.5f));
		for (int32 i = 0; i < 4; i++)
		{
			Reward.Add(0); 
		}
	}
	
	UGameplayStatics::SaveGameToSlot(SaveGame, "Data", 0);
}

void APlayerPawnBase::PlayerDie()
{
	// Disable take damage
	bReceiveDamage = false;

	// Disable Enemy Spawner
	EnableEnemySpawner.Execute(false);

	// Slow world down after player die
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.3f);

	// Disable attack
	GetWorldTimerManager().ClearTimer(AttackTimer);

	// Disable player input movement
	DisableInput(PC);

	// Player Mesh
	VisualMesh->SetVisibility(false);
	PlayerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Save reward and progress
	SaveRewardAndProgress(false);

	// VFX, HUD Animation, Camera Shake, Sound
	UParticleSystemComponent* Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticle, GetActorLocation(), FRotator::ZeroRotator, FVector(1.0f));
	ALevelGameModeBase* GameMode = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	ULevelHUD* HUD = Cast<ULevelHUD>(GameMode->HUDRef);
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), ExplodeCamShake, GetActorLocation(), 0.0f, 1000.0f);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplodeSound, GetActorLocation());
	HUD->PlayAnimation(HUD->HUDAnimation, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.2f);
	Explosion->OnSystemFinished.AddDynamic(HUD, &ULevelHUD::HUDMissionFail);

	// Clear Skill Timer
	if (SkillTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(SkillTimer);
	}
}

void APlayerPawnBase::TouchStartFunction(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Disable other fingers
	if (FingerIndex != ETouchIndex::Touch1)
	{
		return;
	}

	TouchStartPosition = FVector2D(Location.X, Location.Y);
}

void APlayerPawnBase::TouchRepeatFunction(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Disable other fingers
	if (FingerIndex != ETouchIndex::Touch1)
	{
		return;
	}

	const FVector2D CurrentTouchPosition = FVector2D(Location.X, Location.Y);
	const FVector2D Displacement = CurrentTouchPosition - TouchStartPosition;

	if (Displacement.Size() < 1.0f)
	{
		return;
	}
	else
	{
		FVector TouchStartLocation;
		FVector CurrentTouchLocation;
		FVector WorldDirection;
		UGameplayStatics::DeprojectScreenToWorld(PC, TouchStartPosition, TouchStartLocation, WorldDirection);
		UGameplayStatics::DeprojectScreenToWorld(PC, CurrentTouchPosition, CurrentTouchLocation, WorldDirection);
		const FVector WorldDisplacement = CurrentTouchLocation - TouchStartLocation;

		// X-axis
		FVector ActorLocation = GetActorLocation();
		ActorLocation.X += WorldDisplacement.X;
		SetActorLocation(ActorLocation, true);

		// Y-axis
		ActorLocation = GetActorLocation();
		ActorLocation.Y += WorldDisplacement.Y;
		SetActorLocation(ActorLocation, true);

		// Update TouchStartPosition
		TouchStartPosition = CurrentTouchPosition;
	}
}

void APlayerPawnBase::CastSkill()
{
	UE_LOG(LogTemp, Warning, TEXT("Cast Skill"));

}
