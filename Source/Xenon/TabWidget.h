// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TabWidget.generated.h"

/**
 * 
 */

UCLASS()
class XENON_API UTabWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWidgetSwitcher* TabSwitcher;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Item Bar")
	class UTextBlock* GasCanText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Item Bar")
	class UTextBlock* BlueGasCanText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Item Bar")
	class UTextBlock* RedGasCanText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Item Bar")
	class UTextBlock* GreenGasCanText;

	UFUNCTION(BlueprintCallable, Category = "Item Bar")
	void SetupItemBar();

	UFUNCTION(BlueprintCallable, Category = "Item Bar")
	FText ConvertToSIPrefix(const int32 InNumber);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Check Box Text")
	FSlateColor DisableTextColor;

	UPROPERTY(BlueprintReadOnly, Category = "Check Box Text")
	FSlateColor ActiveTextColor;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Check Box Text")
	class UTextBlock* ShopText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Check Box Text")
	class UTextBlock* MainMenuText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Check Box Text")
	class UTextBlock* RefineryText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Check Box")
	class UCheckBox* ShopCheckBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Check Box")
	class UCheckBox* MainMenuCheckBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Check Box")
	class UCheckBox* RefineryCheckBox;

	UFUNCTION(BlueprintCallable, Category = "Check Box Function")
	void OnShopCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Check Box Function")
	void OnMainMenuCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Check Box Function")
	void OnRefineryCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Delegate Function")
	void SwitchTab(int32 Index);
};
