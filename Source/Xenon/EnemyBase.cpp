// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "PlayerPawnBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/BlockingVolume.h"
#include "Components/TimelineComponent.h"

#include "CustomSaveGame.h"
#include "SkillSystem.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup components
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneRoot;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetCollisionProfileName(TEXT("Enemy"));
	EnemyMesh->CastShadow = false;
	EnemyMesh->SetupAttachment(RootComponent);

	DamageTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Damage Timeline"));

	// Death Animation
	AppearanceMin = 0.0f;
	AppearanceMax = 1.0f;

	SpawnGasCanProbability = 0.0f;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	// Variables
	bDead = false;
	DisappearSpeed = (AppearanceMax - AppearanceMin) * 1.5f;
	
	// Adjust enemy MaxHealth, CollideDamage based on level difficulty
	auto AdjustBasedOnDifficulty = [&](void) -> void
	{
		UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
		const float Difficulty = 1 + 0.25 * SaveGame->LevelIndex;

		MaxHealth *= Difficulty;
		CurrentHealth = MaxHealth;

		CollideDamage *= Difficulty;
	};
	AdjustBasedOnDifficulty();

	// Get enemy material for damage/death animation
	TArray<UMaterialInterface*> EnemyMaterialInterface;
	EnemyMesh->GetUsedMaterials(EnemyMaterialInterface);
	for (auto Material : EnemyMaterialInterface)
	{
		EnemyMaterial.Add(UMaterialInstanceDynamic::Create(Material, this));
	}
	EnemyMaterialInterface.Empty();
	
	for (int i = 0; i < EnemyMaterial.Num(); i++) 
	{
		EnemyMesh->SetMaterial(i, EnemyMaterial[i]);
	}

	// Setup collide functionality
	APlayerPawnBase* Player = Cast<APlayerPawnBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	CollideTimerDelegate.BindUObject(Player, &APlayerPawnBase::ReceiveDamage, CollideDamage);
	EnemyMesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::CollideWithPlayer);
	EnemyMesh->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::EndCollideWithPlayer);

	// Damage Animation
	FOnTimelineFloat UpdateDelegate;
	UpdateDelegate.BindDynamic(this, &AEnemyBase::DamageAnimationUpdate);
	DamageTimeline->AddInterpFloat(DamageAnimationCurve, UpdateDelegate);
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Death animation
	if (bDead)
	{
		float Appearance = AppearanceMin;
		for (auto DynamicMaterial : EnemyMaterial)
		{
			DynamicMaterial->GetScalarParameterValue(TEXT("Appearance"), Appearance);
			Appearance += DeltaTime * DisappearSpeed;
			Appearance = FMath::Clamp(Appearance, AppearanceMin, AppearanceMax);
			DynamicMaterial->SetScalarParameterValue(TEXT("Appearance"), Appearance);
		}
		if (Appearance >= AppearanceMax)
		{
			SpawnGasCan();
			Destroy();
		}
	}
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::ReceiveDamage(const float Damage)
{
	if (!bDead)
	{
		CurrentHealth -= Damage;
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);

		if (CurrentHealth == 0.0f)
		{
			EnemyDie();
			return;
		}
		else
		{
			DamageTimeline->PlayFromStart();
			return;
		}
	}
}

void AEnemyBase::EnemyDie()
{
	// Play death animation 
	// Destroy enemy after death animation
	bDead = true;

	// Disable Collision
	EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (CollideTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(CollideTimer);
	}
}

void AEnemyBase::CollideWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerPawnBase* Player = Cast<APlayerPawnBase>(OtherActor);
	if (Player)
	{
		// Apply damage to player
		Player->ReceiveDamage(CollideDamage);
		GetWorldTimerManager().SetTimer(CollideTimer, CollideTimerDelegate, 0.35f, true);

		// Apply damage to enemy itself
		ReceiveDamage(Player->CollideDamage); 
	}
}

void AEnemyBase::EndCollideWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerPawnBase* Player = Cast<APlayerPawnBase>(OtherActor);
	if (Player)
	{
		GetWorldTimerManager().ClearTimer(CollideTimer);
	}
}

void AEnemyBase::SpawnGasCan()
{
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
	USkillSystem* SkillTree = NewObject<USkillSystem>();
	const float Probability = SpawnGasCanProbability * (1 + SkillTree->GetData(13, SaveGame->UpgradeSubjectLevel[13]) / 100);
	if (SkillTree)
	{
		SkillTree->ConditionalBeginDestroy();
	}

	bool bSpawnGasCan = (FMath::FRand() <= Probability) ? true : false;

	if (!bSpawnGasCan)
	{
		return;
	}
	else
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParameter;
			SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			World->SpawnActor<AActor>(GasCanClass, FTransform(GetActorLocation()), SpawnParameter);
		}
	}
}

void AEnemyBase::DamageAnimationUpdate(const float Value)
{
	for (UMaterialInstanceDynamic* Material : EnemyMaterial)
	{
		if (Material)
		{
			Material->SetScalarParameterValue(FName("DamageAppearance"), Value);
		}
	}
}

