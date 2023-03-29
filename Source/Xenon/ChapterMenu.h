// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChapterMenu.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API UChapterMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(BlueprintReadOnly)
	class USwitcherWidget* SwitcherWidgetRef;

	UFUNCTION(BlueprintCallable)
	void SetupWidget(const int32 ChapterIndex);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BackgroundImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMaterialInterface*> BackgroundMaterial;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ChapterNameText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TArray<FLinearColor> ChapterColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TArray<FText> ChapterName;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundBase* EnterLevelSuccess;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundBase* EnterLevelFail;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Button")
	TArray<class UButton*> LevelButtons;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Button")
	class UButton* LevelButton0;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Button")
	class UButton* LevelButton1;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Button")
	class UButton* LevelButton2;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Button")
	class UButton* LevelButton3;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Button")
	class UButton* LevelButton4;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Button")
	class UButton* LevelButton5;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Button")
	class UButton* LevelButton6;

	UFUNCTION(BlueprintCallable)
	void EnterLevelZero();

	UFUNCTION(BlueprintCallable)
	void EnterLevelOne();

	UFUNCTION(BlueprintCallable)
	void EnterLevelTwo();

	UFUNCTION(BlueprintCallable)
	void EnterLevelThree();

	UFUNCTION(BlueprintCallable)
	void EnterLevelFour();

	UFUNCTION(BlueprintCallable)
	void EnterLevelFive();

	UFUNCTION(BlueprintCallable)
	void EnterLevelSix();

	UFUNCTION(BlueprintCallable)
	void OnLevelButtonClick(const int32 InLevelIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, meta = (BindWidgetAnim), Category = "Animation")
	class UWidgetAnimation* LevelButtonAnimation0;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim), Category = "Animation")
	class UWidgetAnimation* LevelButtonAnimation1;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim), Category = "Animation")
	class UWidgetAnimation* LevelButtonAnimation2;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim), Category = "Animation")
	class UWidgetAnimation* LevelButtonAnimation3;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim), Category = "Animation")
	class UWidgetAnimation* LevelButtonAnimation4;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim), Category = "Animation")
	class UWidgetAnimation* LevelButtonAnimation5;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim), Category = "Animation")
	class UWidgetAnimation* LevelButtonAnimation6;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UWidgetAnimation*> LevelButtonAnimation;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UScrollBox* LevelScrollBox;
};
