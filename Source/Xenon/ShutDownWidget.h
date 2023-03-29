// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShutDownWidget.generated.h"

/**
 * 
 */

UCLASS()
class XENON_API UShutDownWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* QuitBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* CancelBtn;

protected:
	UFUNCTION(BlueprintCallable)
	void OnQuitClick();
};
