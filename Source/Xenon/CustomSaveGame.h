// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "Stamina.h"

#include "CustomSaveGame.generated.h"

UCLASS()
class XENON_API UCustomSaveGame : public USaveGame
{
	GENERATED_BODY()
	
private:
	UCustomSaveGame();

public:
	UPROPERTY(BlueprintReadOnly)
	FStamina PlayerStamina;

	/*
	* 0 represents "Blue"
	* 1 represents "Red"
	* 2 represents "Green"
	*/
	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "2"))
	int32 PlayerIndex;

	/*
	* [0] White Can
	* [1] Blue Can
	* [2] Red Can
	* [3] Green Can
	* [4] Dyson Sphere
	* [5] Medkit
	*/
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> ItemData;

	/*
	* [0] Blue Refiner End Time
	* [1] Red Refiner End Time
	* [2] Green Refiner End Time
	*/
	UPROPERTY(BlueprintReadOnly)
	TArray<FDateTime> RefinerEndTime;

	/*
	* [0] Blue RefiningCount
	* [1] Blue RefinedCount
	* [2] Red RefiningCount
	* [3] Red RefinedCount
	* [4] Green RefiningCount
	* [5] Green RefinedCount
	*/
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> Refiner;

	/*
	* [0] Blue Health
	* [1] Blue Attack
	* [2] Blue Attack Speed
	* [3] Blue Skill Value
	* [4] Red Health
	* [5] Red Attack
	* [6] Red Attack Speed
	* [7] Red Skill Value
	* [8] Green Health
	* [9] Green Attack
	* [10] Green Attack Speed
	* [11] Green Attack Value
	* [12] Maximum Stamina
	* [13] Gas Can Drop Rate
	* [14] Refiner Capacity
	* [15] Refine Speed
	*/
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> UpgradeSubjectLevel;

	/*
	* 0 represents "Relic"
	* 1 represents "Jungle"
	* 2 represents "Glacier"
	* 3 represents "Lab"
	* 4 represents "Volcano"
	* 5 represents "Black Hole"
	*/
	UPROPERTY(BlueprintReadOnly)
	int32 ChapterIndex;

	/*
	* Difficulty
	* 0 => 100%
	* 1 => 125%
	* 2 => 150%
	* 3 => 175%
	* 4 => 200%
	* 5 => 225%
	* 6 => 250%
	* const float Difficulty = 1 + 0.25 * LevelIndex;
	*/
	UPROPERTY(BlueprintReadOnly)
	int32 LevelIndex;

	/*
	* [0] "Relic"
	* [1] "Jungle"
	* [2] "Glacier"
	* [3] "Lab"
	* [4] "Volcano"
	* [5] "Black Hole"
	*/
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> ChapterProgress;
};
