// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeMenu.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API UUpgradeMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* UpgradeButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Display")
	class UTextBlock* CurrentLevelText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Display")
	class UTextBlock* MaxLevelText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Display")
	class UTextBlock* WhiteCanText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Display")
	class UTextBlock* BlueCanText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Display")
	class UTextBlock* RedCanText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Display")
	class UTextBlock* GreenCanText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Display")
	class UTextBlock* CurrentValueText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Display")
	class UTextBlock* UpgradeValueText;

	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim), Category = "Animation")
	class UWidgetAnimation* UpgradeSuccessAnimation;

	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim), Category = "Animation")
	class UWidgetAnimation* UpgradeFailAnimation;

	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnimOptional), Category = "Animation")
	class UWidgetAnimation* RefineSpeedFailAnimation;

	UPROPERTY(BlueprintReadOnly)
	class USkillSystemWidget* SkillWidgetRef;

protected:
	UFUNCTION(BlueprintCallable)
	void LoadData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SubjectIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "White, Blue, Red, Green"))
	TArray<int32> UpgradeResourceBase;

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	void Upgrade();
};
