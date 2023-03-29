// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* Button0;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* Button1;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* Button2;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* Button3;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* Button4;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* Button5;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* Button6;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Buttons")
	class UButton* Button7;

protected:
	UFUNCTION(BlueprintCallable)
	void OnFormationClick();

	UFUNCTION(BlueprintCallable)
	void OnTurretClick();

	UFUNCTION(BlueprintCallable)
	void OnLaserClick();

	UFUNCTION(BlueprintCallable)
	void OnLaserTurretClick();

	UFUNCTION(BlueprintCallable)
	void OnMegaClick();

	UFUNCTION(BlueprintCallable)
	void OnAggressiveMegaClick();

	UFUNCTION(BlueprintCallable)
	void OnSineAsteroidClick();

	UFUNCTION(BlueprintCallable)
	void OnSniperClick();

protected:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UUserWidget>> DetailMenus;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UMenuAnchor* DetailMenuAnchor;

	UFUNCTION(BlueprintCallable)
	void ShowDetailMenu();

	UPROPERTY(BlueprintReadOnly)
	int32 DetailMenuIndex;

	UFUNCTION(BlueprintCallable)
	UUserWidget* SelectDetailMenu();

	UFUNCTION(BlueprintCallable)
	void CloseDetailMenu(bool bIsOpen);

	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim))
	class UWidgetAnimation* BlurAnimation;
};
