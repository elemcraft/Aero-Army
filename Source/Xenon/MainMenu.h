// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */

UCLASS()
class XENON_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	class UAudioComponent* ScrollAudio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	class USoundBase* ScrollSound;

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayScrollSound();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme Color")
	TArray<FLinearColor> ThemeColor;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Stamina")
	class UTextBlock* EnergyText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Stamina")
	class UTextBlock* RemainTimeText;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void UpdateStaminaData();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetPlayButtonText();

public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerDataText(const int32 PlayerIndex);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Screen Element")
	class UTextBlock* HealthText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Screen Element")
	class UTextBlock* AttackText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Screen Element")
	class UTextBlock* AttackSpeedText;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Game Over")
	class UMenuAnchor* GameOverMenuAnchor;

	UFUNCTION(BlueprintCallable, Category = "Game Over")
	class UUserWidget* PrepareGameOverWidget();

	UFUNCTION(BlueprintCallable, Category = "Game Over")
	void CloseGameOverWidget();

	UFUNCTION(BlueprintCallable, Category = "Game Over")
	void OnGameOverMenuStateChanged(bool bIsOpen);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Inventory")
	class UMenuAnchor* InventoryMenuAnchor;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	class UUserWidget* PrepareInventoryWidget();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CloseInventoryWidget();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnInventoryMenuStateChanged(bool bIsOpen);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Shut Down")
	class UMenuAnchor* ShutDownMenuAnchor;

	UFUNCTION(BlueprintCallable, Category = "Shut Down")
	class UUserWidget* PrepareShutDownWidget();

	UFUNCTION(BlueprintCallable, Category = "Shut Down")
	void CloseShutDownWidget();

	UFUNCTION(BlueprintCallable, Category = "Shut Down")
	void OnShutDownMenuStateChanged(bool bIsOpen);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "SpaceshipBtn")
	class USpaceshipBtn* SpaceshipBtn0;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "SpaceshipBtn")
	class USpaceshipBtn* SpaceshipBtn1;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "SpaceshipBtn")
	class USpaceshipBtn* SpaceshipBtn2;
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Scroll Box")
	class UButtonScrollBox* BtnScrollBox;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	class UImage* ThunderSkill;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	class UImage* HeartSkill;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	class UImage* ShieldSkill;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void ShowSkillImage(int32 PlayerIndex);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void HideSkillImage(int32 PlayerIndex);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* RightBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* LeftBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* PlayBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* SettingBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* InventoryBtn;

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnRightBtnClick();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnLeftBtnClick();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnPlayClick();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnSettingClick();

	UFUNCTION(BlueprintCallable, Category = "Button Function")
	void OnInventoryClick();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Screen Element")
	class UImage* BarImage_1;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Screen Element")
	class UImage* BarImage_2;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Screen Element")
	class UImage* BarImage_3;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Screen Element")
	class UImage* CircleImage;

	UPROPERTY(BlueprintReadWrite, Category = "Screen Element")
	class UMaterialInstanceDynamic* TechCircle;

	UFUNCTION(BlueprintCallable, Category = "Screen Element")
	void RotateTechCircle(const float InDeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Screen Element")
	void SetScreenElementColor(const int32 Index);

protected:
	UPROPERTY(BlueprintReadOnly)
	class USkillSystem* SkillTree;
};
