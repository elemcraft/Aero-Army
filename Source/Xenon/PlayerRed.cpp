// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerRed.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "CustomSaveGame.h"
#include "SkillSystem.h"
#include "Components/AudioComponent.h"

void APlayerRed::BeginPlay()
{
	Super::BeginPlay();

	auto LoadPlayerData = [&](void) -> void
	{
		UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
		USkillSystem* SkillTree = NewObject<USkillSystem>();

		MaxHealth = SkillTree->GetData(4, SaveGame->UpgradeSubjectLevel[4]);
		CurrentHealth = MaxHealth;
		AttackValue = SkillTree->GetData(5, SaveGame->UpgradeSubjectLevel[5]);
		AttackSpeed = SkillTree->GetData(6, SaveGame->UpgradeSubjectLevel[6]);
		RegeneratePercentage = SkillTree->GetData(7, SaveGame->UpgradeSubjectLevel[7]) / 100.0f;

		if (SkillTree)
		{
			SkillTree->ConditionalBeginDestroy();
		}
	};
	LoadPlayerData();

	UE_LOG(LogTemp, Warning, TEXT("MaxHealth = %f \n AttackValue = %f \n AttackSpeed = %f \n Skill Value = %f"), MaxHealth, AttackValue, AttackSpeed, RegeneratePercentage);

	RegenerateHealth = MaxHealth * RegeneratePercentage;
}

void APlayerRed::CastSkill()
{
	Super::CastSkill(); 

	// Play Skill Sound Effect
	UAudioComponent* SoundEffect = UGameplayStatics::CreateSound2D(GetWorld(), PlayerSkillSound);
	if (SoundEffect)
	{
		SoundEffect->Play();
	}

	// Play regenertate health particle system
	UGameplayStatics::SpawnEmitterAttached(HealBurstParticle, PlayerMesh, NAME_None, FVector(ForceInit), FRotator::ZeroRotator, FVector(0.8f), EAttachLocation::SnapToTarget);

	GeneratedHealth = 0.0f;

	auto Regenerate = [&](void) -> void
	{
		CurrentHealth += RegenerateHealth / 10.0f;
		GeneratedHealth += RegenerateHealth / 10.0f;
		UpdateHealthBar.Broadcast(CurrentHealth / MaxHealth);

		if (GeneratedHealth >= RegenerateHealth)
		{
			if (SkillTimer.IsValid())
			{
				GetWorldTimerManager().ClearTimer(SkillTimer);
			}
		}
	};
	GetWorldTimerManager().SetTimer(SkillTimer, Regenerate, 0.1f, true);
}