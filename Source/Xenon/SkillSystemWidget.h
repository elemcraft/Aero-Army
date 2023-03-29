// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillSystemWidget.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE(FSetItemBar);

UCLASS()
class XENON_API USkillSystemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* SkillTreeCanvas;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* BackBtn;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Delegate")
	FSetItemBar SetItemBar;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Background")
	class UImage* StarBackgroundImage;

	UPROPERTY(BlueprintReadWrite, Category = "Background")
	class UMaterialInstanceDynamic* BGStar;

	UFUNCTION(BlueprintCallable)
	void OnBackBtnClick();

	UFUNCTION(BlueprintCallable, Category = "Background")
	void RotateBackground(const float InDeltaTime);

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Upgrade")
	class UMenuAnchor* UpgradeMenuAnchor;

protected:
	UPROPERTY(EditAnywhere, Category = "Upgrade")
	TArray<TSubclassOf<UUserWidget>> UpgradeMenus;

	UPROPERTY(BlueprintReadOnly, Category = "Upgrade")
	int32 UpgradeMenuIndex;

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	class UUserWidget* ShowUpgradeMenu();

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	void CloseUpgradeMenu(bool bIsOpen);
	
	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim), Category = "Upgrade")
	class UWidgetAnimation* PopUpAnimation;

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<class UCheckBox*> UpgradeCheckBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Blue Upgrade Check Boxes")
	class UCheckBox* BlueHealthUpgrade;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Blue Upgrade Check Boxes")
	class UCheckBox* BlueAttackUpgrade;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Blue Upgrade Check Boxes")
	class UCheckBox* BlueAttackSpeedUpgrade;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Blue Upgrade Check Boxes")
	class UCheckBox* BlueSkillUpgrade;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Red Upgrade Check Boxes")
	class UCheckBox* RedHealthUpgrade;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Red Upgrade Check Boxes")
	class UCheckBox* RedAttackUpgrade;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Red Upgrade Check Boxes")
	class UCheckBox* RedAttackSpeedUpgrade;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Red Upgrade Check Boxes")
	class UCheckBox* RedSkillUpgrade;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Green Upgrade Check Boxes")
	class UCheckBox* GreenHealthUpgrade;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Green Upgrade Check Boxes")
	class UCheckBox* GreenAttackUpgrade;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Green Upgrade Check Boxes")
	class UCheckBox* GreenAttackSpeedUpgrade;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Green Upgrade Check Boxes")
	class UCheckBox* GreenSkillUpgrade;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Global Upgrade Check Boxes")
	class UCheckBox* MaximumStaminaUpgrade;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Global Upgrade Check Boxes")
	class UCheckBox* GasCanDropRateUpgrade;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Global Upgrade Check Boxes")
	class UCheckBox* RefinerCapacityUpgrade;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Global Upgrade Check Boxes")
	class UCheckBox* RefinerSpeedUpgrade;

	UFUNCTION(BlueprintCallable, Category = "Blue Check Box Function")
	void BlueHealthCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Blue Check Box Function")
	void BlueAttackCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Blue Check Box Function")
	void BlueAttackSpeedCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Blue Check Box Function")
	void BlueSkillCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Red Check Box Function")
	void RedHealthCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Red Check Box Function")
	void RedAttackCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Red Check Box Function")
	void RedAttackSpeedCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Red Check Box Function")
	void RedSkillCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Green Check Box Function")
	void GreenHealthCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Green Check Box Function")
	void GreenAttackCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Green Check Box Function")
	void GreenAttackSpeedCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Green Check Box Function")
	void GreenSkillCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Global Check Box Function")
	void MaximumStaminaCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Global Check Box Function")
	void GasCanDropRateCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Global Check Box Function")
	void RefinerCapacityCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Global Check Box Function")
	void RefinerSpeedCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, meta = (BindWidget), Category = "Check Box Function")
	void OnUpgradeCheck();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	class USoundBase* UpgradeSuccessSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	class USoundBase* UpgradeFailSound;

	UFUNCTION(BlueprintCallable)
	void PlayUpgradeSoundEffect(const bool bUpgradeSuccess);

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Item Bar")
	class UTextBlock* GasCanText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Item Bar")
	class UTextBlock* BlueGasCanText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Item Bar")
	class UTextBlock* RedGasCanText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Item Bar")
	class UTextBlock* GreenGasCanText;

	UFUNCTION(BlueprintCallable, Category = "Item Bar")
	void UpdateSkillItemBar();

	UFUNCTION(BlueprintCallable, Category = "Item Bar")
	FText ConvertToSIPrefix(const int32 InNumber);
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSkillTree();
};
