// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceshipEnemyBase.h"
#include "PlayerPawnBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpaceshipEnemyBase::ASpaceshipEnemyBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup components
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneRoot;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetCollisionProfileName(TEXT("Pawn"));
	EnemyMesh->CastShadow = false;
	EnemyMesh->SetupAttachment(RootComponent);

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpaceshipEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	DisappearSpeed = 0.25f;
	bDead = false;
	Appearance = 0.0f;
	UMaterialInterface* Mat = EnemyMesh->GetMaterial(0);
	DyMat = UMaterialInstanceDynamic::Create(Mat, this);
	DyMat->SetScalarParameterValue(TEXT("Appearance"), Appearance);
	EnemyMesh->SetMaterial(0, DyMat);
	
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	CollideDamage = 20.0f;
	ForwardSpeed = 25.0f;

	EnemyMesh->OnComponentBeginOverlap.AddDynamic(this, &ASpaceshipEnemyBase::OverlapBegin);
	EnemyMesh->OnComponentEndOverlap.AddDynamic(this, &ASpaceshipEnemyBase::EndOverlap);
	
	OnTakeAnyDamage.AddDynamic(this, &ASpaceshipEnemyBase::ReceiveDamage);

	GetWorldTimerManager().SetTimer(AttackTimer, this, &ASpaceshipEnemyBase::Attack, 0.35f, true);
}

// Called every frame
void ASpaceshipEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bDead)
	{
		MoveForward(DeltaTime);
	}
	
	if (bDead)
	{
		Appearance += DeltaTime* DisappearSpeed;
		Appearance = FMath::Clamp(Appearance, 0.0f, 1.0f);
		DyMat->SetScalarParameterValue(TEXT("Appearance"), Appearance);
		if (Appearance == 1.0f)
		{
			Destroy();
		}
	}
}

// Called to bind functionality to input
void ASpaceshipEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASpaceshipEnemyBase::ApplyCollideDamage()
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UGameplayStatics::ApplyDamage(Player, CollideDamage, PlayerController, this, UDamageType::StaticClass());
}

void ASpaceshipEnemyBase::MoveForward(const float InDeltaTime)
{
	FVector Location = GetActorLocation();
	Location.X -= InDeltaTime * ForwardSpeed;
	SetActorLocation(Location);
}

void ASpaceshipEnemyBase::Attack()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		World->SpawnActor<AActor>(ProjectileClass, Muzzle->GetComponentTransform(), SpawnParameter);
	}
}

void ASpaceshipEnemyBase::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerPawnBase* Player = Cast<APlayerPawnBase>(OtherActor);
	if (Player)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		// Apply damage to player
		UGameplayStatics::ApplyDamage(Player, CollideDamage, PC, this, UDamageType::StaticClass());
		GetWorldTimerManager().SetTimer(OverlapTimer, this, &ASpaceshipEnemyBase::ApplyCollideDamage, 0.35f, true);
		
		// Apply damage to colliding enemy itself
		UGameplayStatics::ApplyDamage(this, Player->CollideDamage, PC, Player, UDamageType::StaticClass());
	}
}

void ASpaceshipEnemyBase::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerPawnBase* Player = Cast<APlayerPawnBase>(OtherActor);
	if (Player && OverlapTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(OverlapTimer);
	}
}

void ASpaceshipEnemyBase::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	CurrentHealth -= Damage;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);

	if (CurrentHealth == 0)
	{
		EnemyDie();
	}
}

void ASpaceshipEnemyBase::EnemyDie()
{
	bDead = true;

	EnemyMesh->OnComponentBeginOverlap.Clear();
	EnemyMesh->OnComponentEndOverlap.Clear();
	if (OverlapTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(OverlapTimer);
	}
	EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OnTakeAnyDamage.Clear();
	GetWorldTimerManager().ClearTimer(AttackTimer);

	GetController()->Destroy();

	// Spawn gas can
	/*
	bool bSpawnGasCan = (FMath::FRand() >= 0.0f) ? true : false;
	if (bSpawnGasCan == true)
	{
		SpawnGasCan();
	}
	*/
}

void ASpaceshipEnemyBase::SpawnGasCan()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FTransform Transform(GetActorLocation());
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		World->SpawnActor<AActor>(GasCanClass, Transform, SpawnParameter);
	}
}