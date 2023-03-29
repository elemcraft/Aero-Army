// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingWidget.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API USettingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* BackBtn;

protected:
	UFUNCTION(BlueprintCallable)
	void OnBackBtnClick();
};
