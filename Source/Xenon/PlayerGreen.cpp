// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGreen.h"
#include "Kismet/GameplayStatics.h"
#include "CustomSaveGame.h"
#include "SkillSystem.h"
#include "Components/AudioComponent.h"

APlayerGreen::APlayerGreen()
{
	VisualShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual Shield Mesh"));
	VisualShieldMesh->SetVisibility(false);
	VisualShieldMesh->CastShadow = false;
	VisualShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VisualShieldMesh->SetCollisionProfileName(FName("NoCollision"));
	VisualShieldMesh->SetupAttachment(RootComponent);

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield Mesh"));
	ShieldMesh->SetVisibility(false);
	ShieldMesh->CastShadow = false;
	ShieldMesh->SetCollisionProfileName(FName("Player"));
	ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShieldMesh->SetupAttachment(RootComponent);
}

void APlayerGreen::BeginPlay()
{
	Super::BeginPlay();

	auto LoadPlayerData = [&](void) -> void
	{
		UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
		USkillSystem* SkillTree = NewObject<USkillSystem>();

		MaxHealth = SkillTree->GetData(8, SaveGame->UpgradeSubjectLevel[8]);
		CurrentHealth = MaxHealth;
		AttackValue = SkillTree->GetData(9, SaveGame->UpgradeSubjectLevel[9]);
		AttackSpeed = SkillTree->GetData(10, SaveGame->UpgradeSubjectLevel[10]);
		ShieldTime = SkillTree->GetData(11, SaveGame->UpgradeSubjectLevel[11]);

		if (SkillTree)
		{
			SkillTree->ConditionalBeginDestroy();
		}
	};
	LoadPlayerData();

	UE_LOG(LogTemp, Warning, TEXT("MaxHealth = %f \n AttackValue = %f \n AttackSpeed = %f \n Skill Value = %f"), MaxHealth, AttackValue, AttackSpeed, ShieldTime);
}

void APlayerGreen::CastSkill()
{
	Super::CastSkill();
	
	// Play shield burst particle system
	UGameplayStatics::SpawnEmitterAttached(ShieldBurstParticle, PlayerMesh, NAME_None, FVector(ForceInit), FRotator::ZeroRotator, FVector(0.6f), EAttachLocation::SnapToTarget);

	// Play Skill Sound Effect
	UAudioComponent* SoundEffect = UGameplayStatics::CreateSound2D(GetWorld(), PlayerSkillSound);
	if (SoundEffect)
	{
		SoundEffect->Play();
	}

	bReceiveDamage = false;
	VisualShieldMesh->SetVisibility(true);
	ShieldMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	auto DestroyShield = [&](void) -> void
	{
		bReceiveDamage = true;
		VisualShieldMesh->SetVisibility(false);
		ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	};
	
	GetWorldTimerManager().SetTimer(SkillTimer, DestroyShield, ShieldTime, false);
}