// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"
#include "Components/Button.h"

#include "Kismet/GameplayStatics.h"
#include "LevelGameModeBase.h"
#include "LevelHUD.h"

#include "EnemySpawner.h"

#include "PlayerPawnBase.h"
#include "Components/MenuAnchor.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ALevelGameModeBase* GameMode = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	ULevelHUD* HUD = Cast<ULevelHUD>(GameMode->HUDRef);

	if (CancelBtn)
	{
		CancelBtn->OnClicked.AddDynamic(HUD, &ULevelHUD::UnpauseGame);
	}
	if (QuitBtn)
	{
		QuitBtn->OnClicked.AddDynamic(this, &UPauseWidget::OnQuitClick);
	}
}

void UPauseWidget::OnQuitClick()
{
	APlayerPawnBase* Player = Cast<APlayerPawnBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Player->bReceiveDamage = false;

	// Clear Skill Timer
	if (Player->SkillTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(Player->SkillTimer);
	}

	TArray<AActor*> EnemySpawner;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("EnemySpawner"), EnemySpawner);
	Cast<AEnemySpawner>(EnemySpawner[0])->Enable(false);

	ALevelGameModeBase* GameMode = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	ULevelHUD* HUD = Cast<ULevelHUD>(GameMode->HUDRef);

	// Close pause menu anchor
	HUD->PauseMenuAnchor->Close(); 

	// Play Door Animation
	HUD->PlayDoorCloseAnimation();
}
