// Fill out your copyright notice in the Description page of Project Settings.


#include "RefineryWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "CustomSaveGame.h"
#include "Kismet/GameplayStatics.h"

#include "Components/AudioComponent.h"

#include "SkillSystem.h"

void URefineryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto SetupSound = [&](void) -> void
	{
		RefinerButtonSuccess = UGameplayStatics::CreateSound2D(GetWorld(), RefinerButtonSound);
		RefinerButtonSuccess->SetBoolParameter(FName("bSuccess"), true);
		RefinerButtonFail = UGameplayStatics::CreateSound2D(GetWorld(), RefinerButtonSound);
		RefinerButtonFail->SetBoolParameter(FName("bSuccess"), false);
	};
	SetupSound();

	auto BindButtonFunction = [&](void) -> void
	{
		if (BluePlusButton)
		{
			BluePlusButton->OnClicked.AddDynamic(this, &URefineryWidget::OnBluePlusClick);
		}
		if (BlueMinusButton)
		{
			BlueMinusButton->OnClicked.AddDynamic(this, &URefineryWidget::OnBlueMinusClick);
		}
		if (RedPlusButton)
		{
			RedPlusButton->OnClicked.AddDynamic(this, &URefineryWidget::OnRedPlusClick);
		}
		if (RedMinusButton)
		{
			RedMinusButton->OnClicked.AddDynamic(this, &URefineryWidget::OnRedMinusClick);
		}
		if (GreenPlusButton)
		{
			GreenPlusButton->OnClicked.AddDynamic(this, &URefineryWidget::OnGreenPlusClick);
		}
		if (GreenMinusButton)
		{
			GreenMinusButton->OnClicked.AddDynamic(this, &URefineryWidget::OnGreenMinusClick);
		}
		if (BlueCollectBtn)
		{
			BlueCollectBtn->OnClicked.AddDynamic(this, &URefineryWidget::OnBlueCollect);
		}
		if (RedCollectBtn)
		{
			RedCollectBtn->OnClicked.AddDynamic(this, &URefineryWidget::OnRedCollect);
		}
		if (GreenCollectBtn)
		{
			GreenCollectBtn->OnClicked.AddDynamic(this, &URefineryWidget::OnGreenCollect);
		}
	};
	BindButtonFunction();

	RefinerText.Add(BlueRefinerText);
	RefinerText.Add(RedRefinerText);
	RefinerText.Add(GreenRefinerText);

	TimerDisplay.Add(BlueTimerText);
	TimerDisplay.Add(RedTimerText);
	TimerDisplay.Add(GreenTimerText);

	RefinerAnimation.Add(BlueRefinerAnimation);
	RefinerAnimation.Add(RedRefinerAnimation);
	RefinerAnimation.Add(GreenRefinerAnimation);

	RefinedText.Add(BlueRefinedText);
	RefinedText.Add(RedRefinedText);
	RefinedText.Add(GreenRefinedText);

	CollectBtn.Add(BlueCollectBtn);
	CollectBtn.Add(RedCollectBtn);
	CollectBtn.Add(GreenCollectBtn);

	SkillTree = NewObject<USkillSystem>();
}

void URefineryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// Update Data
	UpdateRefinerData();
	
	// Update Refiner Interface
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
	SetRefinerInterface(SaveGame->Refiner, SaveGame->RefinerEndTime[0], 0);
	SetRefinerInterface(SaveGame->Refiner, SaveGame->RefinerEndTime[1], 1);
	SetRefinerInterface(SaveGame->Refiner, SaveGame->RefinerEndTime[2], 2);
}

void URefineryWidget::PlayButtonSuccessSound()
{
	if (RefinerButtonSuccess)
	{
		RefinerButtonSuccess->Activate(true);
		return;
	}
	else
	{
		RefinerButtonSuccess = UGameplayStatics::CreateSound2D(GetWorld(), RefinerButtonSound);
		RefinerButtonSuccess->SetBoolParameter(FName("bSuccess"), true);
		PlayButtonSuccessSound();
	}
}

void URefineryWidget::PlayButtonFailSound()
{
	if (RefinerButtonFail)
	{
		RefinerButtonFail->Activate(true);
		return;
	}
	else
	{
		RefinerButtonFail = UGameplayStatics::CreateSound2D(GetWorld(), RefinerButtonSound);
		RefinerButtonFail->SetBoolParameter(FName("bSuccess"), false);
		PlayButtonFailSound();
	}
}

void URefineryWidget::UpdateRefinerData()
{
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));

	auto ProcessData = [&](TArray<int32>& Refiner, const int32 RefinerIndex) -> void
	{
		if (Refiner[RefinerIndex * 2] != 0)
		{
			FTimespan RemainTime = SaveGame->RefinerEndTime[RefinerIndex] - FDateTime::Now();
			if (RemainTime < 0)
			{
				Refiner[RefinerIndex * 2 + 1] += Refiner[RefinerIndex * 2];
				Refiner[RefinerIndex * 2] = 0;
			}
			else
			{
				// Still Refining
				int32 RemainCan = 0;
				while (RemainTime > 0)
				{
					RemainTime -= FTimespan(0, 0, SkillTree->GetData(15, SaveGame->UpgradeSubjectLevel[15]));
					RemainCan += 1;
				}

				Refiner[RefinerIndex * 2 + 1] += (Refiner[RefinerIndex * 2] - RemainCan);
				Refiner[RefinerIndex * 2] = RemainCan;
			}
		}
	};

	ProcessData(SaveGame->Refiner, 0);
	ProcessData(SaveGame->Refiner, 1);
	ProcessData(SaveGame->Refiner, 2);

	// Save game
	UGameplayStatics::SaveGameToSlot(SaveGame, "Data", 0);
}

void URefineryWidget::SetRefinerInterface(UPARAM(ref) const TArray<int32>& Refiner, const FDateTime& RefinerEndTime, const int32 RefinerIndex)
{
	// Set Refiner Text
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
	const int32 Capacity = int32(SkillTree->GetData(14, SaveGame->UpgradeSubjectLevel[14]));
	const FText Text = FText::Format(FText::FromString("{0} / {1}"), FText::AsNumber(Refiner[RefinerIndex * 2]), FText::AsNumber(Capacity));
	RefinerText[RefinerIndex]->SetText(Text);

	// Set Timer Text
	const FTimespan RemainTime = RefinerEndTime - FDateTime::Now();
	FText TimerText = (RemainTime < 0) ? FText::FromString("00:00:00") : FText::FromString(RemainTime.ToString().RightChop(1).LeftChop(4));
	TimerDisplay[RefinerIndex]->SetText(TimerText); 

	// Set Refined Text
	if (Refiner[RefinerIndex * 2 + 1] != 0)
	{
		RefinedText[RefinerIndex]->SetText(FText::AsNumber(Refiner[RefinerIndex * 2 + 1]));
		if (RefinedText[RefinerIndex]->Visibility != ESlateVisibility::SelfHitTestInvisible)
		{
			RefinedText[RefinerIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	
	// Refining Animation
	auto SetRefinerAnimation = [&](void) -> void
	{
		if (Refiner[RefinerIndex * 2] == 0)
		{
			if (Refiner[RefinerIndex * 2 + 1] == 0)
			{
				CollectBtn[RefinerIndex]->SetVisibility(ESlateVisibility::Collapsed);
			}
			StopAnimation(RefinerAnimation[RefinerIndex]);
			return;
		}
		else if (Refiner[RefinerIndex * 2] >= 1)
		{
			if (!IsAnimationPlaying(RefinerAnimation[RefinerIndex]))
			{
				CollectBtn[RefinerIndex]->SetVisibility(ESlateVisibility::Visible);
				PlayAnimation(RefinerAnimation[RefinerIndex], 0.0f, 0);
			}
			return;
		}
	};
	SetRefinerAnimation();
}

void URefineryWidget::OnBlueCollect()
{
	OnCollectClick(0);
}

void URefineryWidget::OnRedCollect()
{
	OnCollectClick(1);
}

void URefineryWidget::OnGreenCollect()
{
	OnCollectClick(2);
}

void URefineryWidget::OnCollectClick(const int32 RefinerIndex)
{
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));

	SaveGame->ItemData[RefinerIndex + 1] += SaveGame->Refiner[RefinerIndex * 2 + 1];
	SaveGame->Refiner[RefinerIndex * 2 + 1] = 0;

	RefinedText[RefinerIndex]->SetVisibility(ESlateVisibility::Collapsed);

	UGameplayStatics::SaveGameToSlot(SaveGame, "Data", 0);

	SetItemBarText.Execute();
}

bool URefineryWidget::CheckRefineCondition(const int32 RefiningCan, const int32 Capacity)
{
	// Reach refiner capacity
	if (RefiningCan == Capacity)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reach refiner capacity"));
		return false;
	}

	// Out of gas can storage
	const UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
	if (SaveGame->ItemData[0] == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Out of gas can storage"));
		return false;
	}

	return true;
}

void URefineryWidget::OnBluePlusClick()
{
	const bool bSuccess = OnPlusClick(0);
	if (bSuccess)
	{
		SetItemBarText.Execute();
	}
}

void URefineryWidget::OnBlueMinusClick()
{
	const bool bSuccess = OnMinusClick(0);
	if (bSuccess)
	{
		SetItemBarText.Execute();
	}
}

void URefineryWidget::OnRedPlusClick()
{
	const bool bSuccess = OnPlusClick(1);
	if (bSuccess)
	{
		SetItemBarText.Execute();
	}
}

void URefineryWidget::OnRedMinusClick()
{
	const bool bSuccess = OnMinusClick(1);
	if (bSuccess)
	{
		SetItemBarText.Execute();
	}
}

void URefineryWidget::OnGreenPlusClick()
{
	const bool bSuccess = OnPlusClick(2);
	if (bSuccess)
	{
		SetItemBarText.Execute();
	}
}

void URefineryWidget::OnGreenMinusClick()
{
	const bool bSuccess = OnMinusClick(2);
	if (bSuccess)
	{
		SetItemBarText.Execute();
	}
}

bool URefineryWidget::OnPlusClick(const int32 RefinerIndex)
{
	// Load Data
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));

	// Return if "reach refining limit" or "out of gas can storage"
	const int32 Capacity = int32(SkillTree->GetData(14, SaveGame->UpgradeSubjectLevel[14]));
	if (!CheckRefineCondition(SaveGame->Refiner[RefinerIndex * 2], Capacity))
	{
		PlayButtonFailSound();
		return false;
	}

	// Update refiner data
	if (SaveGame->Refiner[RefinerIndex * 2] == 0)
	{
		SaveGame->RefinerEndTime[RefinerIndex] = FDateTime::Now() + FTimespan(0, 0, SkillTree->GetData(15, SaveGame->UpgradeSubjectLevel[15]));
	}
	else
	{
		SaveGame->RefinerEndTime[RefinerIndex] += FTimespan(0, 0, SkillTree->GetData(15, SaveGame->UpgradeSubjectLevel[15]));
	}
	SaveGame->Refiner[RefinerIndex * 2] += 1;

	// Update item data slot
	SaveGame->ItemData[0] -= 1;

	UGameplayStatics::SaveGameToSlot(SaveGame, "Data", 0);

	PlayButtonSuccessSound();

	SetRefinerInterface(SaveGame->Refiner, SaveGame->RefinerEndTime[RefinerIndex], RefinerIndex);
	
	return true;
}

bool URefineryWidget::OnMinusClick(const int32 RefinerIndex)
{
	// Load Data
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));

	// Check condition (if there is any refining gas can)
	if (SaveGame->Refiner[RefinerIndex * 2] == 0)
	{
		PlayButtonFailSound();
		return false;
	}

	// Update refiner data
	SaveGame->Refiner[RefinerIndex * 2] -= 1;
	SaveGame->RefinerEndTime[RefinerIndex] -= FTimespan(0, 0, SkillTree->GetData(15, SaveGame->UpgradeSubjectLevel[15]));

	// Update item data slot
	SaveGame->ItemData[0] += 1;

	UGameplayStatics::SaveGameToSlot(SaveGame, "Data", 0);

	PlayButtonSuccessSound();

	// Visual feedback
	SetRefinerInterface(SaveGame->Refiner, SaveGame->RefinerEndTime[RefinerIndex], RefinerIndex);

	return true;
}
