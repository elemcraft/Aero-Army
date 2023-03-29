// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RewardMenu.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API URewardMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CollectedGasCanText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* DifficultyMultiplierText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ResultText;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* WhiteCanText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* BlueCanText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* RedCanText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* GreenCanText;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UTextBlock*> RewardText;

	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim), Category = "Game Over")
	class UWidgetAnimation* PopUpAnimation;
};
