// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "CustomSaveGame.h"

#include "Components/AudioComponent.h"

#include "LevelHUD.h"

void ALevelGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
	DefaultPawnClass = PlayerPawnClass[SaveGame->PlayerIndex];
}

void ALevelGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Add InGameHUD widget to viewport
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
	AddWidgetToViewport(LevelHUDClass[SaveGame->PlayerIndex]);
}

void ALevelGameModeBase::LoadLobbyLevel()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, TEXT("Lobby"));
	}
}

void ALevelGameModeBase::PlayBackgroundMusic()
{
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
	int32 MusicIndex;
	if (SaveGame->ChapterIndex == 5)
	{
		MusicIndex = 0;
	}
	else if (SaveGame->ChapterIndex == 3)
	{
		MusicIndex = 1;
	}
	else
	{
		MusicIndex = 2;
	}

	UAudioComponent* Music = UGameplayStatics::CreateSound2D(GetWorld(), BackgroundMusic[MusicIndex]);
	if (Music)
	{
		Music->FadeIn(2.0f);
	}
}

void ALevelGameModeBase::AddWidgetToViewport(UPARAM(ref) const TSubclassOf<class UUserWidget>& WidgetClass)
{
	if (WidgetClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			HUDRef = CreateWidget<ULevelHUD>(World, WidgetClass);
			if (HUDRef)
			{
				HUDRef->AddToViewport();
			}
		}
	}
}