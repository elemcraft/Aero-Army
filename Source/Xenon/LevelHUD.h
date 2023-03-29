// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelHUD.generated.h"

/**
 * 
 */

UCLASS()
class XENON_API ULevelHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere)
	FLinearColor PlayerColor; 

	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	class UMaterialInstanceDynamic* SkillIconDynamic;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Skill")
	class UButton* SkillBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Skill")
	class UImage* SkillButtonFrame;
	
	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	FTimerHandle SkillRefillTimer;

	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	float SkillPercentage;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SkillRefill();

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void OnSkillClick();

public:
	UFUNCTION(BlueprintCallable, Category = "Game Over")
	void HUDMissionFail(class UParticleSystemComponent* PSystem);

	UFUNCTION(BlueprintCallable, Category = "Game Over")
	void HUDMissionSuccess();

protected:
	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim), Category = "Game Over")
	class UWidgetAnimation* GameOverAnimation;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Game Over")
	class UTextBlock* MissionStateText;

protected:
	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim), Category = "Ready")
	class UWidgetAnimation* ReadyAnimation;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Health")
	class UProgressBar* PlayerHealthBar;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdatePlayerHealthBar(const float InHealth);

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void PlayDamageAnimation();

	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim), Category = "Damage")
	class UWidgetAnimation* DamageAnimation;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Item")
	class UTextBlock* ItemCount;

	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim), Category = "Item")
	class UWidgetAnimation* CollectAnimation;

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetItemCountText(const int32 InCount);

protected:
	UFUNCTION(BlueprintCallable, Category = "HUDAnim")
	void PlayHUDAnim();

public:
	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim), Category = "HUDAnim")
	class UWidgetAnimation* HUDAnimation;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Pause")
	bool bPauseGame;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Pause")
	class UCheckBox* PauseCheckBox;

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void OnPauseCheck(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void OnMenuStateChanged(bool bIsOpen);

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Pause")
	class UMenuAnchor* PauseMenuAnchor;

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void UnpauseGame();

public:
	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim), Category = "DoorAnim")
	class UWidgetAnimation* DoorAnimation;

	UFUNCTION(BlueprintCallable)
	void PlayDoorCloseAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	class USoundBase* DoorOpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	class USoundBase* DoorCloseSound;

	UFUNCTION(BlueprintCallable)
	void PlayReadyAnimation();

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* GameProgressBar;

	UFUNCTION(BlueprintCallable)
	void UpdateGameProgressBar(float InPercent);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UMenuAnchor* RewardMenuAnchor;

	UFUNCTION(BlueprintCallable)
	void ShowRewardMenu();

	UFUNCTION(BlueprintCallable)
	void OnRewardMenuClose(bool bIsOpen);

public:
	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim))
	class UWidgetAnimation* BossHealthBarAnimation;

	UFUNCTION(BlueprintCallable)
	void PlayBossHealthBarAnimation();

public:
	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim))
	class UWidgetAnimation* BossDeathAnimation;

	UFUNCTION(BlueprintCallable)
	void PlayBossDeathAnimation();
};
