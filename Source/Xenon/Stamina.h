// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Stamina.generated.h"

USTRUCT(BlueprintType)
struct FStamina
{
	GENERATED_BODY()

	FORCEINLINE FStamina();

	explicit FORCEINLINE FStamina(const int32 InEnergy);

	UPROPERTY(BlueprintReadWrite)
	FDateTime EndTime = FDateTime(2021, 12, 9, 0, 0, 0, 0);

	UPROPERTY(BlueprintReadWrite)
	int32 Energy;

	UPROPERTY(BlueprintReadWrite)
	FTimespan RestoreTime = FTimespan(0, 3, 0);
};

FORCEINLINE FStamina::FStamina()
{
}
/*
FORCEINLINE FStamina::FStamina(const int32 InEnergy) :Energy(InEnergy)
{
}
*/
FORCEINLINE FStamina::FStamina(const int32 InEnergy) 
{
	Energy = InEnergy;
}
