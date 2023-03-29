// Fill out your copyright notice in the Description page of Project Settings.


#include "RewardMenu.h"
#include "Components/TextBlock.h"

#include "Kismet/GameplayStatics.h"
#include "PlayerPawnBase.h"

void URewardMenu::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerPawnBase* Player = Cast<APlayerPawnBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	RewardText.Add(CollectedGasCanText);
	RewardText.Add(DifficultyMultiplierText);
	RewardText.Add(ResultText);
	RewardText.Add(WhiteCanText);
	RewardText.Add(BlueCanText);
	RewardText.Add(RedCanText);
	RewardText.Add(GreenCanText);

	for (int32 i = 0; i < RewardText.Num(); i++)
	{
		RewardText[i]->SetText(FText::AsNumber(Player->Reward[i]));
	}

	Player->Reward.Empty();
	RewardText.Empty();

	PlayAnimation(PopUpAnimation);
}