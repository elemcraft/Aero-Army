// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* CancelBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* QuitBtn;

protected:
	UFUNCTION(BlueprintCallable)
	void OnQuitClick();
};
