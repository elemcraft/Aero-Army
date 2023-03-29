// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillSystem.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API USkillSystem : public UObject
{
	GENERATED_BODY()

public:
	USkillSystem();
	
public:
	UFUNCTION(BlueprintCallable)
	float GetData(const int32 SubjectIndex, const int32 InLevel);

	UFUNCTION(BlueprintCallable)
	int32 GetMaxLevel(const int32 SubjectIndex);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Blue", meta = (Tooltip = "SubjectIndex = 0"))
	TArray<float> BlueHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Blue", meta = (Tooltip = "SubjectIndex = 1"))
	TArray<float> BlueAttack;

	UPROPERTY(BlueprintReadOnly, Category = "Blue", meta = (Tooltip = "SubjectIndex = 2"))
	TArray<float> BlueAttackSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Blue", meta = (Tooltip = "SubjectIndex = 3"))
	TArray<float> BlueSkillValue;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Red", meta = (Tooltip = "SubjectIndex = 4"))
	TArray<float> RedHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Red", meta = (Tooltip = "SubjectIndex = 5"))
	TArray<float> RedAttack;

	UPROPERTY(BlueprintReadOnly, Category = "Red", meta = (Tooltip = "SubjectIndex = 6"))
	TArray<float> RedAttackSpeed;

	// Percentage
	UPROPERTY(BlueprintReadOnly, Category = "Red", meta = (Tooltip = "SubjectIndex = 7"))
	TArray<float> RedSkillValue;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Green", meta = (Tooltip = "SubjectIndex = 8"))
	TArray<float> GreenHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Green", meta = (Tooltip = "SubjectIndex = 9"))
	TArray<float> GreenAttack;

	UPROPERTY(BlueprintReadOnly, Category = "Green", meta = (Tooltip = "SubjectIndex = 10"))
	TArray<float> GreenAttackSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Green", meta = (Tooltip = "SubjectIndex = 11"))
	TArray<float> GreenSkillValue;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Global", meta = (Tooltip = "SubjectIndex = 12"))
	TArray<float> MaximumStamina;

	// Percentage
	UPROPERTY(BlueprintReadOnly, Category = "Global", meta = (Tooltip = "SubjectIndex = 13"))
	TArray<float> GasCanDropRate;

	UPROPERTY(BlueprintReadOnly, Category = "Global", meta = (Tooltip = "SubjectIndex = 14"))
	TArray<float> RefinerCapacity;

	// Seconds
	UPROPERTY(BlueprintReadOnly, Category = "Global", meta = (Tooltip = "SubjectIndex = 15"))
	TArray<float> RefinerSpeed;
};
