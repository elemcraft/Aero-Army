// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelHUD.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/HUD.h"
#include "PlayerPawnBase.h"
#include "Components/MenuAnchor.h"
#include "PauseWidget.h"

#include "CustomSaveGame.h"

#include "Components/Image.h"
#include "Components/Border.h"

#include "LevelGameModeBase.h"

#include "Components/AudioComponent.h"

void ULevelHUD::NativeConstruct()
{
	Super::NativeConstruct();

	// Setup player health bar delegate
	APlayerPawnBase* Player = Cast<APlayerPawnBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Player->UpdateHealthBar.AddDynamic(this, &ULevelHUD::UpdatePlayerHealthBar);
	
	auto SetupSkillButton = [&](void) -> void
	{
		// Skill button parameters
		SkillBtn->SetIsEnabled(false);
		SkillPercentage = 0.3f;

		// Set skill button material
		SkillIconDynamic = UMaterialInstanceDynamic::Create(Cast<UMaterialInterface>(SkillBtn->WidgetStyle.Normal.GetResourceObject()), this);
		SkillIconDynamic->SetScalarParameterValue(TEXT("Percentage"), SkillPercentage);
		SkillBtn->WidgetStyle.Normal.SetResourceObject(SkillIconDynamic);
		SkillBtn->WidgetStyle.Hovered.SetResourceObject(SkillIconDynamic);
		SkillBtn->WidgetStyle.Pressed.SetResourceObject(SkillIconDynamic);
		SkillBtn->WidgetStyle.Disabled.SetResourceObject(SkillIconDynamic);

		// Start skill countdown
		GetWorld()->GetTimerManager().SetTimer(SkillRefillTimer, this, &ULevelHUD::SkillRefill, 0.2f, true);

		// Skill button binding
		SkillBtn->OnClicked.AddDynamic(this, &ULevelHUD::OnSkillClick);
	};
	SetupSkillButton();

	auto SetupItemText = [&](void) -> void
	{
		// Initialization
		SetItemCountText(0);

		// Bind delegate
		APlayerPawnBase* Player = Cast<APlayerPawnBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		Player->UpdateGasCanCount.AddDynamic(this, &ULevelHUD::SetItemCountText);
	};
	SetupItemText();

	auto SetupPauseFunctionality = [&](void) -> void
	{
		bPauseGame = false;
		PauseMenuAnchor->OnMenuOpenChanged.AddDynamic(this, &ULevelHUD::OnMenuStateChanged);
		PauseCheckBox->OnCheckStateChanged.AddDynamic(this, &ULevelHUD::OnPauseCheck);
	};
	SetupPauseFunctionality();

	// Setup reward menu anchor
	RewardMenuAnchor->OnMenuOpenChanged.AddDynamic(this, &ULevelHUD::OnRewardMenuClose);

	// Bind player receive damage animation delegate
	Player->TakeDamageAnimation.BindDynamic(this, &ULevelHUD::PlayDamageAnimation);
	
	// Play player entrance animation and HUD animation after ready animation
	FWidgetAnimationDynamicEvent PlayEntranceAnim;
	FWidgetAnimationDynamicEvent PlayHUDAnim;
	PlayEntranceAnim.BindDynamic(Player, &APlayerPawnBase::PlayEntranceAnimation);
	PlayHUDAnim.BindDynamic(this, &ULevelHUD::PlayHUDAnim);
	BindToAnimationFinished(ReadyAnimation, PlayEntranceAnim);
	BindToAnimationFinished(ReadyAnimation, PlayHUDAnim);
	
	// Play ready animation after door open animation
	FWidgetAnimationDynamicEvent AnimationFinished;
	AnimationFinished.BindDynamic(this, &ULevelHUD::PlayReadyAnimation);
	BindToAnimationFinished(DoorAnimation, AnimationFinished);

	// Play door open animation
	FTimerHandle DoorOpenDelay;
	auto PlayDoorOpenAnimation = [&](void) -> void
	{
		PlayAnimation(DoorAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 2.0f);
		GetWorld()->GetTimerManager().ClearTimer(DoorOpenDelay);

		// Open door sound effect
		UAudioComponent* SoundEffect = UGameplayStatics::CreateSound2D(GetWorld(), DoorOpenSound);
		SoundEffect->Play();
	};
	GetWorld()->GetTimerManager().SetTimer(DoorOpenDelay, PlayDoorOpenAnimation, 0.5f, false);
}

void ULevelHUD::OnPauseCheck(bool bIsChecked)
{	
	// Only triggered when game is not paused
	if (bIsChecked && !bPauseGame)
	{
		bPauseGame = true;

		// Pop up pause widget
		PauseMenuAnchor->SetVisibility(ESlateVisibility::Visible);
		PauseMenuAnchor->Open(true);

		// Pause game
		UGameplayStatics::SetGamePaused(GetWorld(), bPauseGame);
	}
}

void ULevelHUD::UnpauseGame()
{
	// Uncheck pause check box
	PauseCheckBox->SetIsChecked(false);

	// Close pause menu anchor
	PauseMenuAnchor->Close();

	// Unpause game
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	PauseMenuAnchor->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	bPauseGame = false;
}

void ULevelHUD::OnMenuStateChanged(bool bIsOpen)
{
	// Only triggered when closing menu
	if (!bIsOpen && bPauseGame)
	{
		UnpauseGame();
	}
}

void ULevelHUD::UpdatePlayerHealthBar(const float InHealthRatio)
{
	PlayerHealthBar->SetPercent(InHealthRatio);
}

void ULevelHUD::PlayDamageAnimation()
{
	PlayAnimation(DamageAnimation);
}

void ULevelHUD::SetItemCountText(const int32 InCount)
{
	ItemCount->SetText(FText::AsNumber(InCount));
	PlayAnimation(CollectAnimation);
}

void ULevelHUD::PlayHUDAnim()
{
	PlayAnimation(HUDAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.8f);
}

void ULevelHUD::SkillRefill()
{
	SkillPercentage += 0.008f;
	SkillPercentage = FMath::Clamp(SkillPercentage, 0.0f, 1.0f);
	SkillIconDynamic->SetScalarParameterValue(TEXT("Percentage"), SkillPercentage);

	// Skill refill complete
	if (SkillPercentage == 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(SkillRefillTimer);
		SkillButtonFrame->SetBrushTintColor(PlayerColor);
		SkillBtn->SetIsEnabled(true);
	}
}

void ULevelHUD::OnSkillClick()
{
	SkillPercentage = 0;
	SkillIconDynamic->SetScalarParameterValue(TEXT("Percentage"), SkillPercentage);
	SkillButtonFrame->SetBrushTintColor(FLinearColor(FColor(169.0f, 169.0f, 169.0f)));
	SkillBtn->SetIsEnabled(false);
	GetWorld()->GetTimerManager().SetTimer(SkillRefillTimer, this, &ULevelHUD::SkillRefill, 0.2f, true);

	APlayerPawnBase* Player = Cast<APlayerPawnBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Player->CastSkill();
}

void ULevelHUD::HUDMissionFail(UParticleSystemComponent* PSystem)
{
	MissionStateText->SetText(FText::FromString("FAIL"));

	FWidgetAnimationDynamicEvent AnimationFinished;
	AnimationFinished.BindDynamic(this, &ULevelHUD::ShowRewardMenu);
	BindToAnimationFinished(GameOverAnimation, AnimationFinished);

	PlayAnimation(GameOverAnimation);
}

void ULevelHUD::HUDMissionSuccess()
{
	MissionStateText->SetText(FText::FromString("COMPLETE"));

	FWidgetAnimationDynamicEvent AnimationFinished;
	AnimationFinished.BindDynamic(this, &ULevelHUD::ShowRewardMenu);
	BindToAnimationFinished(GameOverAnimation, AnimationFinished);

	PlayAnimation(GameOverAnimation);
}

void ULevelHUD::PlayDoorCloseAnimation()
{
	// Switch to Lobby Level after door close animation
	FWidgetAnimationDynamicEvent AnimationFinished;
	ALevelGameModeBase* GameMode = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	AnimationFinished.BindDynamic(GameMode, &ALevelGameModeBase::LoadLobbyLevel);
	BindToAnimationFinished(DoorAnimation, AnimationFinished);

	PlayAnimation(DoorAnimation, 0.0f, 1, EUMGSequencePlayMode::Reverse, 2.0f);

	// Door sound effect
	UAudioComponent* SoundEffect = UGameplayStatics::CreateSound2D(GetWorld(), DoorCloseSound);
	SoundEffect->Play();
}

void ULevelHUD::PlayReadyAnimation()
{
	PlayAnimation(ReadyAnimation);
}

void ULevelHUD::UpdateGameProgressBar(float InPercent)
{
	if (GameProgressBar)
	{
		GameProgressBar->SetPercent(InPercent);
	}
}

void ULevelHUD::ShowRewardMenu()
{
	RewardMenuAnchor->Open(true);
}

void ULevelHUD::OnRewardMenuClose(bool bIsOpen)
{
	if (!bIsOpen)
	{
		PlayDoorCloseAnimation();
	}
}

void ULevelHUD::PlayBossHealthBarAnimation()
{
	GameProgressBar->SetFillColorAndOpacity(FLinearColor(0.34f, 0.0f, 1.0, 1.0f));
	PlayAnimation(BossHealthBarAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 0.8f);
}

void ULevelHUD::PlayBossDeathAnimation()
{
	PlayAnimation(BossDeathAnimation);
}
