// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBlue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "CustomSaveGame.h"
#include "SkillSystem.h"
#include "Components/AudioComponent.h"

void APlayerBlue::BeginPlay()
{
	Super::BeginPlay();

	auto LoadPlayerData = [&](void) -> void
	{
		UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
		USkillSystem* SkillTree = NewObject<USkillSystem>();

		MaxHealth = SkillTree->GetData(0, SaveGame->UpgradeSubjectLevel[0]);
		CurrentHealth = MaxHealth;
		AttackValue = SkillTree->GetData(1, SaveGame->UpgradeSubjectLevel[1]);
		AttackSpeed = SkillTree->GetData(2, SaveGame->UpgradeSubjectLevel[2]);
		ElectricDamage = SkillTree->GetData(3, SaveGame->UpgradeSubjectLevel[3]);

		if (SkillTree)
		{
			SkillTree->ConditionalBeginDestroy();
		}
	};
	LoadPlayerData();
	
	UE_LOG(LogTemp, Warning, TEXT("MaxHealth = %f \n AttackValue = %f \n AttackSpeed = %f \n Skill Value = %f"), MaxHealth, AttackValue, AttackSpeed, ElectricDamage);

	ElectricTimes = 0;
	ElectricRadius = 250.0f;
}

void APlayerBlue::CastSkill()
{
	Super::CastSkill();

	// Play shock wave particle system
	UGameplayStatics::SpawnEmitterAttached(ElectricBurstParticle, PlayerMesh, NAME_None, FVector(ForceInit), FRotator::ZeroRotator, FVector(0.8f), EAttachLocation::SnapToTarget);

	// Find enemies in range
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery10);   // Enemy is the tenth object type
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	TArray<AActor*> ActorInRange;
	bool bIsEnemyInRange = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), ElectricRadius, ObjectTypes, AEnemyBase::StaticClass(), ActorsToIgnore, ActorInRange);
	
	// Cast all actor in range to AEnemyBase
	TArray<AEnemyBase*> EnemyInRange;
	for (AActor* Enemy : ActorInRange)
	{
		EnemyInRange.Add(Cast<AEnemyBase>(Enemy));
	}
	ActorInRange.Empty();

	if (bIsEnemyInRange)
	{
		// Spawn  electric arc emitter
		for (int i = 0; i < EnemyInRange.Num(); i++)
		{
			UParticleSystemComponent* Arc = UGameplayStatics::SpawnEmitterAttached(ElectricArcParticle, PlayerMesh, NAME_None, FVector(ForceInit), FRotator::ZeroRotator, EAttachLocation::SnapToTargetIncludingScale);
			ElectricArc.Add(Arc);
			ElectricArc[ElectricArc.Num() - 1]->SetBeamTargetPoint(0, EnemyInRange[i]->GetActorLocation(), 0);
		}

		// Play Skill Sound Effect
		UAudioComponent* SoundEffect = UGameplayStatics::CreateSound2D(GetWorld(), PlayerSkillSound);
		if (SoundEffect)
		{
			SoundEffect->Play();
		}

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &APlayerBlue::ApplyElectricDamage, EnemyInRange);
		GetWorldTimerManager().SetTimer(SkillTimer, TimerDelegate, 0.25f, true, 0.0f);
	}
}

void APlayerBlue::ApplyElectricDamage(UPARAM(ref) TArray<class AEnemyBase*> Enemies)
{
	// Update electric arc target point if the enemy is still alive
	for (int i = 0; i < Enemies.Num(); i++)
	{
		if (Enemies[i])
		{
			// Make sure enemy still exist before reach for bDead variable to avoid crash
			if (!Enemies[i]->bDead)
			{
				ElectricArc[i]->SetBeamTargetPoint(0, Enemies[i]->GetActorLocation(), 0);
				Enemies[i]->ReceiveDamage(ElectricDamage);
			}
			else
			{
				if (ElectricArc[i])
				{
					ElectricArc[i]->DestroyComponent();
				}
				continue;
			}
		}
		else
		{
			if (ElectricArc[i])
			{
				ElectricArc[i]->DestroyComponent();
			}
			continue;
		}
	}

	ElectricTimes += 1;
	if (ElectricTimes >= 10)
	{
		Enemies.Empty();

		// Clear elctric arc
		for (UParticleSystemComponent* Arc : ElectricArc)
		{
			if (Arc)
			{
				Arc->DestroyComponent();
			}
		}
		ElectricArc.Empty();

		ElectricTimes = 0;
		GetWorldTimerManager().ClearTimer(SkillTimer);
	}
}