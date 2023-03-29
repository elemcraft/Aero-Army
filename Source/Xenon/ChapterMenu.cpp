// Fill out your copyright notice in the Description page of Project Settings.


#include "ChapterMenu.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "Kismet/GameplayStatics.h"
#include "CustomSaveGame.h"
#include "SwitcherWidget.h"

#include "Components/ScrollBox.h"

void UChapterMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (LevelButton0)
	{
		LevelButton0->OnClicked.AddDynamic(this, &UChapterMenu::EnterLevelZero);
	}
	if (LevelButton1)
	{
		LevelButton1->OnClicked.AddDynamic(this, &UChapterMenu::EnterLevelOne);
	}
	if (LevelButton2)
	{
		LevelButton2->OnClicked.AddDynamic(this, &UChapterMenu::EnterLevelTwo);
	}
	if (LevelButton3)
	{
		LevelButton3->OnClicked.AddDynamic(this, &UChapterMenu::EnterLevelThree);
	}
	if (LevelButton4)
	{
		LevelButton4->OnClicked.AddDynamic(this, &UChapterMenu::EnterLevelFour);
	}
	if (LevelButton5)
	{
		LevelButton5->OnClicked.AddDynamic(this, &UChapterMenu::EnterLevelFive);
	}
	if (LevelButton6)
	{
		LevelButton6->OnClicked.AddDynamic(this, &UChapterMenu::EnterLevelSix);
	}
	
	LevelButtons.Add(LevelButton0);
	LevelButtons.Add(LevelButton1);
	LevelButtons.Add(LevelButton2);
	LevelButtons.Add(LevelButton3);
	LevelButtons.Add(LevelButton4);
	LevelButtons.Add(LevelButton5);
	LevelButtons.Add(LevelButton6);

	LevelButtonAnimation.Add(LevelButtonAnimation0);
	LevelButtonAnimation.Add(LevelButtonAnimation1);
	LevelButtonAnimation.Add(LevelButtonAnimation2);
	LevelButtonAnimation.Add(LevelButtonAnimation3);
	LevelButtonAnimation.Add(LevelButtonAnimation4);
	LevelButtonAnimation.Add(LevelButtonAnimation5);
	LevelButtonAnimation.Add(LevelButtonAnimation6);
}

void UChapterMenu::SetupWidget(const int32 ChapterIndex)
{
	// Setup widget appearance
	ChapterNameText->SetText(ChapterName[ChapterIndex]);
	ChapterNameText->SetColorAndOpacity(ChapterColor[ChapterIndex]);

	// Set background image material
	BackgroundImage->SetBrushFromMaterial(BackgroundMaterial[ChapterIndex]);

	// Restore scroll box
	LevelScrollBox->SetScrollOffset(0.0f);

	// Disable level button that is inaccessible
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
	const int32 Progress = SaveGame->ChapterProgress[ChapterIndex];
	for (int32 i = 0; i < LevelButtons.Num(); i++)
	{
		if (i > Progress)
		{
			LevelButtons[i]->SetIsEnabled(false);
		}
		else
		{
			LevelButtons[i]->SetIsEnabled(true);
		}
	}
}

void UChapterMenu::EnterLevelZero()
{
	OnLevelButtonClick(0);
}

void UChapterMenu::EnterLevelOne()
{
	OnLevelButtonClick(1);
}

void UChapterMenu::EnterLevelTwo()
{
	OnLevelButtonClick(2);
}

void UChapterMenu::EnterLevelThree()
{
	OnLevelButtonClick(3);
}

void UChapterMenu::EnterLevelFour()
{
	OnLevelButtonClick(4);
}

void UChapterMenu::EnterLevelFive()
{
	OnLevelButtonClick(5);
}

void UChapterMenu::EnterLevelSix()
{
	OnLevelButtonClick(6);
}

void UChapterMenu::OnLevelButtonClick(const int32 InLevelIndex)
{
	// Level Stamina
	const int32 LevelStamina = 8 + InLevelIndex * 2;

	// Check if player has enough energy
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
	if (SaveGame->PlayerStamina.Energy < LevelStamina)
	{
		// Fail sound effect
		PlaySound(EnterLevelFail);
		return;
	}

	// Avoid spamming level button
	for (UButton* Button : LevelButtons)
	{
		Button->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	// Success sound effect
	PlaySound(EnterLevelSuccess);

	// Save level index
	SaveGame->LevelIndex = InLevelIndex;

	// Save stamina data
	SaveGame->PlayerStamina.Energy -= LevelStamina;
	if (SaveGame->PlayerStamina.EndTime > FDateTime::Now())
	{
		SaveGame->PlayerStamina.EndTime += LevelStamina * SaveGame->PlayerStamina.RestoreTime;
	}
	else
	{
		SaveGame->PlayerStamina.EndTime = FDateTime::Now() + LevelStamina * SaveGame->PlayerStamina.RestoreTime;
	}

	UGameplayStatics::SaveGameToSlot(SaveGame, "Data", 0);

	// Level Button Animation
	for (int32 i = 0; i < LevelButtonAnimation.Num(); i++)
	{
		if (i != InLevelIndex)
		{
			PlayAnimation(LevelButtonAnimation[i]);
		}
	}
	
	// Enetr level
	SwitcherWidgetRef->ToBattleLevel();
}

