// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "SwitcherWidget.h"
#include "TabWidget.h"
#include "CustomSaveGame.h"

//#include "Kismet/KismetSystemLibrary.h"

void ALobbyGameMode::StartPlay()
{
	Super::StartPlay();

	// Load Interstitial Ad
	UKismetSystemLibrary::LoadInterstitialAd(0);

	// Create save game object
	auto CreateSaveGame = [&](void) -> void
	{
		const bool bSaveGameExist = UGameplayStatics::DoesSaveGameExist("Data", 0);
		if (bSaveGameExist)
		{
			return;
		}
		else
		{
			UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::CreateSaveGameObject(UCustomSaveGame::StaticClass()));

			SaveGame->PlayerStamina = FStamina(100);

			SaveGame->PlayerIndex = 0;

			SaveGame->ItemData.Init(15, 6);
			SaveGame->ItemData[4] = 0;
			SaveGame->ItemData[5] = 0;
			
			SaveGame->RefinerEndTime.Init(FDateTime(2021, 12, 9, 0, 0, 0, 0), 3);
			SaveGame->Refiner.Init(0, 6);

			SaveGame->ChapterIndex = 0;

			SaveGame->UpgradeSubjectLevel.Init(0, 16);

			SaveGame->LevelIndex = 0;

			SaveGame->ChapterProgress.Init(0, 6);

			UGameplayStatics::SaveGameToSlot(SaveGame, "Data", 0);
		}
	};
	CreateSaveGame();

	// Bind delegate
	GetWidgetDelegate.BindDynamic(this, &ALobbyGameMode::GetIndexWidget);
	ChangeWidget.BindDynamic(this, &ALobbyGameMode::SwitchWidget);
	InMainMenu.BindDynamic(this, &ALobbyGameMode::IsInMainMenu);
	GetCurrentWidgetIndex.BindDynamic(this, &ALobbyGameMode::GetActiveWidgetIndex);
		
	SpawnMenuActors();
	AddSwitcherWidgetToViewport(SwitcherWidgetClass);
}

UWidget* ALobbyGameMode::GetIndexWidget(int32 Index)
{
	return LobbyWidgetSwitcher->GetWidgetAtIndex(Index);
}

void ALobbyGameMode::SwitchWidget(int32 Index)
{
	LobbyWidgetSwitcher->SetActiveWidgetIndex(Index);
}

bool ALobbyGameMode::IsInMainMenu()
{
	if (TabSwitcher->GetActiveWidgetIndex() == 1)
	{
		return true;
	}

	return false;
}

int32 ALobbyGameMode::GetActiveWidgetIndex()
{
	return LobbyWidgetSwitcher->GetActiveWidgetIndex();
}

void ALobbyGameMode::AddSwitcherWidgetToViewport(UPARAM(ref) const TSubclassOf<class USwitcherWidget>& WidgetClass)
{
	if (WidgetClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			USwitcherWidget* Widget = CreateWidget<USwitcherWidget>(World, WidgetClass);
			if (Widget)
			{
				LobbyWidgetSwitcher = Widget->LobbyWidgetSwitcher;
				TabSwitcher = Cast<UTabWidget>(LobbyWidgetSwitcher->GetWidgetAtIndex(0))->TabSwitcher;
				Widget->AddToViewport();
			}
		}
	}
}

void ALobbyGameMode::SpawnMenuActors()
{
	UWorld* World = GetWorld();
	if (World)
	{
		// Spawn main menu actors at (0, 0, 0)
		FTransform Origin;
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		for (auto ActorClass : MenuActorClassArray)
		{
			World->SpawnActor<AActor>(ActorClass, Origin, SpawnParameter);
		}
	}
}