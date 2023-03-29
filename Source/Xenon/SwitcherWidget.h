// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SwitcherWidget.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API USwitcherWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWidgetSwitcher* LobbyWidgetSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	class USoundBase* DoorOpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	class USoundBase* DoorCloseSound;

	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim), Category = "DoorAnim")
	class UWidgetAnimation* DoorAnimation;

	UFUNCTION(BlueprintCallable)
	void ToBattleLevel();

	UFUNCTION(BlueprintCallable)
	void LevelTransition();

protected:
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle AnimDelayTimer;

	UFUNCTION(BlueprintCallable)
	void PlayDoorAnimation();

protected:
	UFUNCTION(BlueprintCallable, Category = "Interstitial Ad")
	void ShowAd();

	UPROPERTY(BlueprintReadOnly, Category = "Interstitial Ad")
	int32 ShowAdAttempt;

	UPROPERTY(BlueprintReadOnly, Category="Interstitial Ad")
	FTimerHandle AdTimer;
};
