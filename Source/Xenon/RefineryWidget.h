// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RefineryWidget.generated.h"

/**
 * 
 */

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE(FSetItemBarText);

UCLASS()
class XENON_API URefineryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	class UAudioComponent* RefinerButtonFail;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	class UAudioComponent* RefinerButtonSuccess;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	class USoundBase* RefinerButtonSound;

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayButtonSuccessSound();

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayButtonFailSound();

public:
	UPROPERTY(BlueprintReadWrite, Category = "Delegate")
	FSetItemBarText SetItemBarText;

protected:
	UFUNCTION(BlueprintCallable, Category = "Data")
	void UpdateRefinerData();

protected:
	UFUNCTION(BlueprintCallable, Category = "Interface")
	void SetRefinerInterface(UPARAM(ref) const TArray<int32>& Refiner, const FDateTime& RefinerEndTime, const int32 RefinerIndex);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Interface")
	class UTextBlock* BlueRefinerText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Interface")
	class UTextBlock* RedRefinerText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Interface")
	class UTextBlock* GreenRefinerText;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UTextBlock*> RefinerText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Interface")
	class UTextBlock* BlueTimerText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Interface")
	class UTextBlock* RedTimerText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Interface")
	class UTextBlock* GreenTimerText;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UTextBlock*> TimerDisplay;

	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim), Category = "Interface")
	class UWidgetAnimation* BlueRefinerAnimation;

	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim), Category = "Interface")
	class UWidgetAnimation* RedRefinerAnimation;

	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim), Category = "Interface")
	class UWidgetAnimation* GreenRefinerAnimation;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UWidgetAnimation*> RefinerAnimation;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Interface")
	class UTextBlock* BlueRefinedText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Interface")
	class UTextBlock* RedRefinedText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Interface")
	class UTextBlock* GreenRefinedText;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UTextBlock*> RefinedText;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* BlueCollectBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* RedCollectBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* GreenCollectBtn;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UButton*> CollectBtn;

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnBlueCollect();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnRedCollect();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnGreenCollect();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnCollectClick(const int32 RefinerIndex);

protected:
	UFUNCTION(BlueprintCallable)
	bool CheckRefineCondition(const int32 RefiningCan, const int32 Capacity);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* BluePlusButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* BlueMinusButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* RedPlusButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* RedMinusButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* GreenPlusButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* GreenMinusButton;

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnBluePlusClick();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnBlueMinusClick();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnRedPlusClick();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnRedMinusClick();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnGreenPlusClick();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnGreenMinusClick();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	bool OnPlusClick(const int32 RefinerIndex);

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	bool OnMinusClick(const int32 RefinerIndex);

	UPROPERTY(BlueprintReadOnly)
	class USkillSystem* SkillTree;

protected:
	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim), Category = "Pattern")
	class UWidgetAnimation* PatternAnimation;

	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim), Category = "Floating")
	class UWidgetAnimation* FloatingAnimation;
};
