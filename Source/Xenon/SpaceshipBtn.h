// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "SpaceshipBtn.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API USpaceshipBtn : public UButton
{
	GENERATED_BODY()

private:
	USpaceshipBtn();
	
protected:
	UFUNCTION(BlueprintCallable)
	void OnButtonClick();
};
