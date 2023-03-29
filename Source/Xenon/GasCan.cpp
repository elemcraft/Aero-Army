// Fill out your copyright notice in the Description page of Project Settings.


#include "GasCan.h"
#include "PlayerPawnBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
AGasCan::AGasCan()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneRoot;

	CanMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Can Mesh"));
	CanMesh->CastShadow = false;
	CanMesh->SetupAttachment(RootComponent);

	RotateSpeed = 20.0f;
}

// Called when the game starts or when spawned
void AGasCan::BeginPlay()
{
	Super::BeginPlay();
	
	RotateDirectionPercentage = FMath::FRand();
	CanMesh->OnComponentBeginOverlap.AddDynamic(this, &AGasCan::OverlapBegin);
}

// Called every frame
void AGasCan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Floating(DeltaTime);
}

void AGasCan::Floating(const float InDeltaTime)
{
	AddActorWorldRotation(FRotator(RotateSpeed * InDeltaTime * RotateDirectionPercentage, 0, RotateSpeed * InDeltaTime * (1 - RotateDirectionPercentage)));
}

void AGasCan::Collected()
{
	// Sound Effect
	UAudioComponent* SoundEffect = UGameplayStatics::CreateSound2D(GetWorld(), CollectedSound);
	if (SoundEffect)
	{
		SoundEffect->Activate(true);
	}

	Destroy();
}

void AGasCan::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerPawnBase* Player = Cast<APlayerPawnBase>(OtherActor);
	if (Player)
	{
		Player->GasCanCount += 1;
		Player->UpdateGasCanCount.Broadcast(Player->GasCanCount);
		Collected();
	}
}
