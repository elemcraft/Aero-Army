// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelMenu.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API ULevelMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Button")
	class UButton* BackBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Button")
	class UButton* RelicButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Button")
	class UButton* JungleButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Button")
	class UButton* GlacierButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Button")
	class UButton* LabButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Button")
	class UButton* VolcanoButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Button")
	class UButton* BlackHoleButton;

protected:
	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnRelicClick();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnJungleClick();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnGlacierClick();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnLabClick();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnVolcanoClick();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnBlackHoleClick();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnChapterBtnClick(const int32 ChapterIndex);

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnBackBtnClick();
};
