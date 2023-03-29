// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* GasCanText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* BlueGasCanText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* RedGasCanText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* GreenGasCanText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DysonSphereText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MedkitText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* CloseBtn;
};
