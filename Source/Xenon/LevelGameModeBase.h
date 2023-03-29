// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LevelGameModeBase.generated.h"

/**
 * 
 */

UCLASS()
class XENON_API ALevelGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void StartPlay() override;

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

public:
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	class ULevelHUD* HUDRef;

	UFUNCTION(BlueprintCallable, Category = "Game Over")
	void LoadLobbyLevel();

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayBackgroundMusic();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Player Pawn")
	TArray<TSubclassOf<class AActor>> PlayerPawnClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TArray<TSubclassOf<class UUserWidget>> LevelHUDClass;

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void AddWidgetToViewport(UPARAM(ref) const TSubclassOf<class UUserWidget>& WidgetClass);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	TArray<class USoundBase*> BackgroundMusic;
};
