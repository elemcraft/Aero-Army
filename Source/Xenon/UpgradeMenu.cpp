// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeMenu.h"

#include "CustomSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/AudioComponent.h"
#include "SkillSystemWidget.h"

#include "MainMenu.h"
#include "LobbyGameMode.h"
#include "TabWidget.h"
#include "Components/WidgetSwitcher.h"
#include "ButtonScrollBox.h"

#include "SkillSystem.h"

void UUpgradeMenu::NativeConstruct()
{
	Super::NativeConstruct();

	LoadData();

	UpgradeButton->OnClicked.AddDynamic(this, &UUpgradeMenu::Upgrade);
}

void UUpgradeMenu::LoadData()
{
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
	const int32 CurrentLevel = SaveGame->UpgradeSubjectLevel[SubjectIndex];
	USkillSystem* SkillTree = NewObject<USkillSystem>();
	
	CurrentLevelText->SetText(FText::FromString("Level " + FString::FromInt(CurrentLevel)));
	MaxLevelText->SetText(FText::FromString("Max Level " + FString::FromInt(SkillTree->GetMaxLevel(SubjectIndex))));

	WhiteCanText->SetText(FText::FromString(FString::FromInt(UpgradeResourceBase[0] * (CurrentLevel + 1))));
	BlueCanText->SetText(FText::FromString(FString::FromInt(UpgradeResourceBase[1] * (CurrentLevel + 1))));
	RedCanText->SetText(FText::FromString(FString::FromInt(UpgradeResourceBase[2] * (CurrentLevel + 1))));
	GreenCanText->SetText(FText::FromString(FString::FromInt(UpgradeResourceBase[3] * (CurrentLevel + 1))));

	CurrentValueText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), SkillTree->GetData(SubjectIndex, SaveGame->UpgradeSubjectLevel[SubjectIndex]))));
	if (CurrentLevel == SkillTree->GetMaxLevel(SubjectIndex))
	{
		// Reach Maximum Level
		UpgradeValueText->SetText(FText::FromString("(MAX)"));
	}
	else
	{
		UpgradeValueText->SetText(FText::FromString("(" + FString::Printf(TEXT("%.1f"), SkillTree->GetData(SubjectIndex, SaveGame->UpgradeSubjectLevel[SubjectIndex] + 1)) + ")"));
	}

	if (SkillTree)
	{
		SkillTree->ConditionalBeginDestroy();
	}
}

void UUpgradeMenu::Upgrade()
{
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
	const int32 CurrentLevel = SaveGame->UpgradeSubjectLevel[SubjectIndex];

	USkillSystem* SkillTree = NewObject<USkillSystem>();

	// Check if reach maximum level
	const bool bReachMaxLevel = CurrentLevel >= SkillTree->GetMaxLevel(SubjectIndex);
	if (bReachMaxLevel)
	{
		SkillWidgetRef->PlayUpgradeSoundEffect(false);
		PlayAnimation(UpgradeFailAnimation, 0.0f, 2, EUMGSequencePlayMode::Forward, 3.0f);
		return;
	}

	if (SkillTree)
	{
		SkillTree->ConditionalBeginDestroy();
	}

	// Calculate required resource to upgrade
	TArray<int32> RequiredResource;
	for (int32 Num : UpgradeResourceBase)
	{
		RequiredResource.Add(Num * (CurrentLevel + 1));
	}

	// Check if player has enough resource
	for (int32 i = 0; i < RequiredResource.Num(); i++)
	{
		SaveGame->ItemData[i] -= RequiredResource[i];
		if (SaveGame->ItemData[i] < 0)
		{
			SkillWidgetRef->PlayUpgradeSoundEffect(false);
			PlayAnimation(UpgradeFailAnimation, 0.0f, 2, EUMGSequencePlayMode::Forward, 3.0f);
			SkillWidgetRef->UpdateSkillItemBar();
			return;
		}
	}
	RequiredResource.Empty();

	// Cannot upgrade refine speed when refiners are not empty
	if (SubjectIndex == 15)
	{
		for (int32 Num : SaveGame->Refiner)
		{
			if (Num != 0)
			{
				SkillWidgetRef->PlayUpgradeSoundEffect(false);
				PlayAnimation(RefineSpeedFailAnimation, 0.0f, 2, EUMGSequencePlayMode::Forward, 3.0f);
				return;
			}
		}
	}

	// Save upgrade to data
	SaveGame->UpgradeSubjectLevel[SubjectIndex] += 1;
	UGameplayStatics::SaveGameToSlot(SaveGame, "Data", 0);

	// Update item bar in tab widget
	SkillWidgetRef->SetItemBar.Execute();

	// Update item bar in skill widget
	SkillWidgetRef->UpdateSkillItemBar();

	// Update upgrade menu display text
	LoadData();

	// Upgrade success sound effect
	SkillWidgetRef->PlayUpgradeSoundEffect(true);

	// Upgrade success animation
	PlayAnimation(UpgradeSuccessAnimation);

	// Update main menu display text
	UMainMenu* MainMenu = Cast<UMainMenu>(Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->TabSwitcher->GetWidgetAtIndex(1));
	if (SubjectIndex / 4 == MainMenu->BtnScrollBox->PlayerIndex)
	{
		MainMenu->SetPlayerDataText(MainMenu->BtnScrollBox->PlayerIndex);
	}
}
