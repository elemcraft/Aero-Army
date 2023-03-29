// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelMenu.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGameMode.h"
#include "Components/Button.h"
#include "ChapterMenu.h"
#include "Components/Image.h"
#include "CustomSaveGame.h"


void ULevelMenu::NativeConstruct()
{
	Super::NativeConstruct();

	auto BindButtonFunction = [&](void) -> void
	{
		if (BackBtn)
		{
			BackBtn->OnClicked.AddDynamic(this, &ULevelMenu::OnBackBtnClick);
		}
		if (RelicButton)
		{
			RelicButton->OnClicked.AddDynamic(this, &ULevelMenu::OnRelicClick);
		}
		if (JungleButton)
		{
			JungleButton->OnClicked.AddDynamic(this, &ULevelMenu::OnJungleClick);
		}
		if (GlacierButton)
		{
			GlacierButton->OnClicked.AddDynamic(this, &ULevelMenu::OnGlacierClick);
		}
		if (LabButton)
		{
			LabButton->OnClicked.AddDynamic(this, &ULevelMenu::OnLabClick);
		}
		if (VolcanoButton)
		{
			VolcanoButton->OnClicked.AddDynamic(this, &ULevelMenu::OnVolcanoClick);
		}
		if (BlackHoleButton)
		{
			BlackHoleButton->OnClicked.AddDynamic(this, &ULevelMenu::OnBlackHoleClick);
		}
	};
	BindButtonFunction();
}

void ULevelMenu::OnRelicClick()
{
	const int32 ChapterIndex = 0;
	OnChapterBtnClick(ChapterIndex);
}

void ULevelMenu::OnJungleClick()
{
	const int32 ChapterIndex = 1;
	OnChapterBtnClick(ChapterIndex);
}

void ULevelMenu::OnGlacierClick()
{
	const int32 ChapterIndex = 2;
	OnChapterBtnClick(ChapterIndex);
}

void ULevelMenu::OnLabClick()
{
	const int32 ChapterIndex = 3;
	OnChapterBtnClick(ChapterIndex);
}

void ULevelMenu::OnVolcanoClick()
{
	const int32 ChapterIndex = 4;
	OnChapterBtnClick(ChapterIndex);
}

void ULevelMenu::OnBlackHoleClick()
{
	const int32 ChapterIndex = 5;
	OnChapterBtnClick(ChapterIndex);
}

void ULevelMenu::OnChapterBtnClick(const int32 ChapterIndex)
{
	// Save ChapterIndex
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
	SaveGame->ChapterIndex = ChapterIndex;
	UGameplayStatics::SaveGameToSlot(SaveGame, "Data", 0);

	// Setup Chapter Menu
	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	UChapterMenu* ChapterMenu = Cast<UChapterMenu>(GameMode->GetWidgetDelegate.Execute(4));
	ChapterMenu->SetupWidget(ChapterIndex);

	// Switch To Chapter Menu
	GameMode->ChangeWidget.Execute(4);
}

void ULevelMenu::OnBackBtnClick()
{
	const ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->ChangeWidget.Execute(0);
}