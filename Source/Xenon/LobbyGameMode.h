// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_OneParam(FChangeWidget, int32, WidgetIndex);

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_OneParam(FChangeTab, int32, WidgetIndex);

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(UWidget*, FGetWidgetDelegate, int32, WidgetIndex);

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_RetVal(bool, FInMainMenu);

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_RetVal(int32, FGetCurrentWidgetIndex);

UCLASS()
class XENON_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void StartPlay() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Delegate Function")
	UWidget* GetIndexWidget(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Delegate Function")
	void SwitchWidget(int32 Index);
	
	UFUNCTION(BlueprintCallable, Category = "Delegate Function")
	bool IsInMainMenu();

	UFUNCTION(BlueprintCallable, Category = "Delegate Function")
	int32 GetActiveWidgetIndex();

public:
	UPROPERTY(BlueprintReadOnly, Category = "Delegate")
	FGetWidgetDelegate GetWidgetDelegate;

	UPROPERTY(BlueprintReadOnly, Category = "Delegate")
	FChangeWidget ChangeWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Delegate")
	FChangeTab ChangeTab;

	UPROPERTY(BlueprintReadOnly, Category = "Delegate")
	FInMainMenu InMainMenu;

	UPROPERTY(BlueprintReadOnly, Category = "Delegate")
	FGetCurrentWidgetIndex GetCurrentWidgetIndex;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Switcher")
	class UWidgetSwitcher* LobbyWidgetSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Switcher")
	class UWidgetSwitcher* TabSwitcher;

protected:
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void AddSwitcherWidgetToViewport(UPARAM(ref) const TSubclassOf<class USwitcherWidget>& WidgetClass);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<class USwitcherWidget> SwitcherWidgetClass;

	UPROPERTY(EditAnywhere, Category = "MenuActors")
	TArray<TSubclassOf<class AActor>> MenuActorClassArray;

	UFUNCTION(BlueprintCallable, Category = "MenuActors")
	void SpawnMenuActors();
};
