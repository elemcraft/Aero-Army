// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ScrollBox.h"
#include "ButtonScrollBox.generated.h"

/**
 * 
 */

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_OneParam(FHideSkillImage, int32, Index);

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_OneParam(FShowSkillImage, int32, Index);

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_OneParam(FChangeScreenElementColor, int32, Index);

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_OneParam(FChangePlayerDataText, int32, Index);

UCLASS()
class XENON_API UButtonScrollBox : public UScrollBox
{
	GENERATED_BODY()

private:
	UButtonScrollBox();

public:
	UPROPERTY(BlueprintReadOnly, Category = "Delegate")
	FHideSkillImage HideSkillImage;

	UPROPERTY(BlueprintReadOnly, Category = "Delegate")
	FShowSkillImage ShowSkillImage;

	UPROPERTY(BlueprintReadOnly, Category = "Delegate")
	FChangeScreenElementColor ChangeScreenElementColor;

	UPROPERTY(BlueprintReadOnly, Category = "Delegate")
	FChangePlayerDataText ChangePlayerDataText;

	// Test
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snap")
	float SnapSpeed = 3.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Snap")
	float SwipeLength;

	UPROPERTY(EditAnywhere, Category = "Snap")
	float ButtonSize;

	UFUNCTION(BlueprintCallable, Category = "Snap")
	void ScrollBoxSnap(const float DeltaTime);

	UPROPERTY(BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "2"), Category = "Snap")
	int32 PlayerIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Snap")
	bool bSnapping;

	UPROPERTY(BlueprintReadWrite, Category = "Snap")
	float ScrollGridLocation;
};
