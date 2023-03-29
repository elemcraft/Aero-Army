// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserBeam.h"
#include "PlayerPawnBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "CustomSaveGame.h"

// Sets default values
ALaserBeam::ALaserBeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	BeamMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	BeamMesh->SetCollisionProfileName(TEXT("Projectile"));
	BeamMesh->SetupAttachment(RootComponent);

	BeamLength = 800.0f;
}

// Called when the game starts or when spawned
void ALaserBeam::BeginPlay()
{
	Super::BeginPlay();

	// Adjust laser damage based on level difficulty
	auto AdjustLaserDamage = [&](void) -> void
	{
		UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
		const float Difficulty = 1 + 0.25 * SaveGame->LevelIndex;
		LaserDamage *= Difficulty;
	};
	AdjustLaserDamage();

	// Variables
	StretchSpeed = 150.0f;
	bStretching = true;
	bHitPlayer = false;
	SourcePoint = GetActorLocation();
	ForwardDirection = GetActorForwardVector();
	
	BeamMesh->OnComponentBeginOverlap.AddDynamic(this, &ALaserBeam::HitPlayer);
	BeamMesh->OnComponentEndOverlap.AddDynamic(this, &ALaserBeam::EndOverlap);
}

// Called every frame
void ALaserBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bStretching)
	{
		ElongateLaser(DeltaTime);
	}
	
	if (bHitPlayer)
	{
		CutBeamLength();
	}
}

void ALaserBeam::HitPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerPawnBase* Player = Cast<APlayerPawnBase>(OtherActor);
	if (Player)
	{
		bHitPlayer = true;
		
		Player->ReceiveDamage(LaserDamage);
		if (Player->CurrentHealth > 0)
		{
			FTimerDelegate DamageDelegate;	
			DamageDelegate.BindUObject(Player, &APlayerPawnBase::ReceiveDamage, LaserDamage);
			GetWorldTimerManager().SetTimer(ApplyDamageTimer, DamageDelegate, 0.2f, true);
		}
	}
}

void ALaserBeam::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerPawnBase* Player = Cast<APlayerPawnBase>(OtherActor);
	if (Player)
	{
		bHitPlayer = false;
		bStretching = true;
		GetWorldTimerManager().ClearTimer(ApplyDamageTimer);
	}
}
/*
void ALaserBeam::OldElongateLaser(const float InDeltaTime)
{
	// Update scale
	FVector Scale = BeamMesh->GetComponentScale();
	Scale.X += InDeltaTime * StretchSpeed;
	Scale.X = FMath::Clamp(Scale.X, 0.1f, 8.0f);
	BeamMesh->SetWorldScale3D(Scale);

	// Update translation
	FVector Translation = SourcePoint;
	Translation.X = SourcePoint.X - 100.0f * Scale.X / 2.0f;
	BeamMesh->SetWorldLocation(Translation);

	// Reach maximum length
	if (Scale.X >= 8.0f)
	{
		bStretching = false;
	}
}

void ALaserBeam::OldCutBeamLength()
{
	// Line trace
	const FVector EndPoint = SourcePoint - FVector(800, 0, 0);
	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(), SourcePoint, EndPoint, FName("Player"), true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
	//FVector PlayerLocation = HitResult.ImpactPoint - FVector(1, 0, 0);
	const FVector PlayerLocation = HitResult.ImpactPoint;

	// Update Scale
	FVector Scale = BeamMesh->GetComponentScale();
	Scale.X = FMath::Abs(SourcePoint.X - PlayerLocation.X) / 100.0f;
	BeamMesh->SetWorldScale3D(Scale);

	// Update translation
	FVector Translation = SourcePoint;
	Translation.X = SourcePoint.X - 100.0f * Scale.X / 2.0f;
	BeamMesh->SetWorldLocation(Translation);
}
*/
void ALaserBeam::CutBeamLength()
{
	// Line trace
	const FVector EndPoint = SourcePoint + ForwardDirection * BeamLength;
	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(), SourcePoint, EndPoint, FName("Player"), true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
	const FVector PlayerLocation = HitResult.ImpactPoint;

	// Update Scale
	FVector Scale = GetActorScale();
	Scale.X = (SourcePoint - PlayerLocation).Size() / 10;
	SetActorScale3D(Scale);

	// Update translation
	const FVector Translation = (SourcePoint + PlayerLocation) / 2;
	SetActorLocation(Translation);
}

void ALaserBeam::ElongateLaser(const float InDeltaTime)
{
	// Update scale
	FVector Scale = GetActorScale();
	Scale.X += InDeltaTime * StretchSpeed;
	Scale.X = FMath::Clamp(Scale.X, 0.8f, 80.0f);
	SetActorScale3D(Scale);

	// Update translation
	const FVector Translation = SourcePoint + ForwardDirection * (Scale.X * 10) / 2;
	SetActorLocation(Translation);

	// Reach maximum length
	if (Scale.X >= 80.0f)
	{
		bStretching = false;
	}
}
