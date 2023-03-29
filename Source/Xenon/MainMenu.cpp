// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/MenuAnchor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ButtonScrollBox.h"
#include "SpaceshipBtn.h"
#include "LobbyGameMode.h"
#include "CustomSaveGame.h"
#include "ShutDownWidget.h"
#include "InventoryWidget.h"
#include "GameOverWidget.h"
#include "Components/AudioComponent.h"

#include "SkillSystem.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	SkillTree = NewObject<USkillSystem>();

	auto BindButtonFunction = [&](void) -> void
	{
		if (RightBtn)
		{
			RightBtn->OnClicked.AddDynamic(this, &UMainMenu::OnRightBtnClick);
		}
		if (LeftBtn)
		{
			LeftBtn->OnClicked.AddDynamic(this, &UMainMenu::OnLeftBtnClick);
		}
		if (PlayBtn)
		{
			PlayBtn->OnClicked.AddDynamic(this, &UMainMenu::OnPlayClick);
		}
		if (SettingBtn)
		{
			SettingBtn->OnClicked.AddDynamic(this, &UMainMenu::OnSettingClick);
		}
		if (InventoryBtn)
		{
			InventoryBtn->OnClicked.AddDynamic(this, &UMainMenu::OnInventoryClick);
		}
	};
	BindButtonFunction();

	// Bind scroll box delegate
	auto BindScrollBoxDelegate = [&](void) -> void
	{
		BtnScrollBox->HideSkillImage.BindDynamic(this, &UMainMenu::HideSkillImage);
		BtnScrollBox->ShowSkillImage.BindDynamic(this, &UMainMenu::ShowSkillImage);
		BtnScrollBox->ChangeScreenElementColor.BindDynamic(this, &UMainMenu::SetScreenElementColor);
		BtnScrollBox->ChangePlayerDataText.BindDynamic(this, &UMainMenu::SetPlayerDataText);
	};
	BindScrollBoxDelegate();

	// Bind ShutDownMenuAnchor delegate
	ShutDownMenuAnchor->OnGetUserMenuContentEvent.BindDynamic(this, &UMainMenu::PrepareShutDownWidget);
	ShutDownMenuAnchor->OnMenuOpenChanged.AddDynamic(this, &UMainMenu::OnShutDownMenuStateChanged);

	// Bind InventoryWidget delegate
	InventoryMenuAnchor->OnGetUserMenuContentEvent.BindDynamic(this, &UMainMenu::PrepareInventoryWidget);
	InventoryMenuAnchor->OnMenuOpenChanged.AddDynamic(this, &UMainMenu::OnInventoryMenuStateChanged);

	auto SetupMainMenu = [&](void) -> void
	{
		// Load Player Index from data
		UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));

		// Scroll box
		BtnScrollBox->PlayerIndex = SaveGame->PlayerIndex;
		BtnScrollBox->ScrollGridLocation = BtnScrollBox->PlayerIndex * BtnScrollBox->ButtonSize;
		BtnScrollBox->SetScrollOffset(BtnScrollBox->ScrollGridLocation);

		// Set player data text
		SetPlayerDataText(BtnScrollBox->PlayerIndex);

		// Set initial skill icon image
		ShowSkillImage(BtnScrollBox->PlayerIndex);

		// Set initial element color
		SetScreenElementColor(BtnScrollBox->PlayerIndex);
	};
	SetupMainMenu();

	// Get TechCircle dynamic material 
	TechCircle = CircleImage->GetDynamicMaterial();

	// Sound
	auto SetupSound = [&](void) -> void
	{
		ScrollAudio = UGameplayStatics::CreateSound2D(GetWorld(), ScrollSound);
	};
	SetupSound();
}

void UMainMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	BtnScrollBox->ScrollBoxSnap(InDeltaTime);

	RotateTechCircle(InDeltaTime);

	UpdateStaminaData();

	SetPlayButtonText();
}

void UMainMenu::PlayScrollSound()
{
	if (ScrollAudio)
	{
		ScrollAudio->Activate(true);
		return;
	}
	else
	{
		ScrollAudio = UGameplayStatics::CreateSound2D(GetWorld(), ScrollSound);
		PlayScrollSound();
	}
}

void UMainMenu::UpdateStaminaData()
{
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot(("Data"), 0));

	FTimespan RemainTime = SaveGame->PlayerStamina.EndTime - FDateTime::Now();
	if (RemainTime <= 0)
	{
		// Fully restore energy
		SaveGame->PlayerStamina.Energy = SkillTree->GetData(12, SaveGame->UpgradeSubjectLevel[12]);
	}
	else
	{
		int32 UsedEnergy = 0;
		while (RemainTime > 0)
		{
			RemainTime -= SaveGame->PlayerStamina.RestoreTime;
			UsedEnergy += 1;
		}
		SaveGame->PlayerStamina.Energy = SkillTree->GetData(12, SaveGame->UpgradeSubjectLevel[12]) - UsedEnergy;
	}

	// Save Game
	UGameplayStatics::SaveGameToSlot(SaveGame, "Data", 0);
}

void UMainMenu::SetPlayButtonText()
{
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));

	const FText Text = FText::Format(FText::FromString("{0} / {1}"), FText::AsNumber(SaveGame->PlayerStamina.Energy), FText::AsNumber(SkillTree->GetData(12, SaveGame->UpgradeSubjectLevel[12])));
	EnergyText->SetText(Text);

	FTimespan RemainTime = SaveGame->PlayerStamina.EndTime - FDateTime::Now();
	FText TimeText;
	if (RemainTime < 0)
	{
		TimeText = FText::FromString("00:00");
	}
	else
	{
		RemainTime %= SaveGame->PlayerStamina.RestoreTime;
		TimeText = FText::FromString(RemainTime.ToString().RightChop(4).LeftChop(4));
	}
	RemainTimeText->SetText(TimeText);
}

void UMainMenu::SetPlayerDataText(const int32 PlayerIndex)
{
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));

	const float DisplayHealth = SkillTree->GetData(PlayerIndex * 4, SaveGame->UpgradeSubjectLevel[PlayerIndex * 4]);
	const float DisplayAttack = SkillTree->GetData(PlayerIndex * 4 + 1, SaveGame->UpgradeSubjectLevel[PlayerIndex * 4 + 1]);
	const float DisplayAttackSpeed = SkillTree->GetData(PlayerIndex * 4 + 2, SaveGame->UpgradeSubjectLevel[PlayerIndex * 4 + 2]);

	HealthText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), DisplayHealth)));
	AttackText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), DisplayAttack)));
	AttackSpeedText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), DisplayAttackSpeed)));
}

UUserWidget* UMainMenu::PrepareGameOverWidget()
{
	GameOverMenuAnchor->SetVisibility(ESlateVisibility::Visible);

	UGameOverWidget* Widget = CreateWidget<UGameOverWidget>(GetWorld(), GameOverMenuAnchor->MenuClass);
	Widget->CloseBtn->OnClicked.AddDynamic(this, &UMainMenu::CloseGameOverWidget);

	return Widget;
}

void UMainMenu::CloseGameOverWidget()
{
	GameOverMenuAnchor->Close();
}

void UMainMenu::OnGameOverMenuStateChanged(bool bIsOpen)
{
	// Triggered when closing menu
	if (!bIsOpen)
	{
		GameOverMenuAnchor->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

UUserWidget* UMainMenu::PrepareInventoryWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("Inv Open"));

	InventoryMenuAnchor->SetVisibility(ESlateVisibility::Visible);

	UInventoryWidget* Widget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryMenuAnchor->MenuClass);

	const UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
	
	// Set text
	Widget->GasCanText->SetText(FText::AsNumber(SaveGame->ItemData[0]));
	Widget->BlueGasCanText->SetText(FText::AsNumber(SaveGame->ItemData[1]));
	Widget->RedGasCanText->SetText(FText::AsNumber(SaveGame->ItemData[2]));
	Widget->GreenGasCanText->SetText(FText::AsNumber(SaveGame->ItemData[3]));
	Widget->DysonSphereText->SetText(FText::AsNumber(SaveGame->ItemData[4]));
	Widget->MedkitText->SetText(FText::AsNumber(SaveGame->ItemData[5]));

	// Bind close widget delegate
	Widget->CloseBtn->OnClicked.AddDynamic(this, &UMainMenu::CloseInventoryWidget);

	return Widget;
}

void UMainMenu::CloseInventoryWidget()
{
	InventoryMenuAnchor->Close();
}

void UMainMenu::OnInventoryMenuStateChanged(bool bIsOpen)
{
	// Triggered when closing menu
	if (!bIsOpen)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inv Close"));
		InventoryMenuAnchor->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

UUserWidget* UMainMenu::PrepareShutDownWidget()
{
	ShutDownMenuAnchor->SetVisibility(ESlateVisibility::Visible);

	UShutDownWidget* Widget = CreateWidget<UShutDownWidget>(GetWorld(), ShutDownMenuAnchor->MenuClass);
	Widget->CancelBtn->OnClicked.AddDynamic(this, &UMainMenu::CloseShutDownWidget);

	return Widget;
}

void UMainMenu::CloseShutDownWidget()
{
	ShutDownMenuAnchor->Close();
}

void UMainMenu::OnShutDownMenuStateChanged(bool bIsOpen)
{
	// Triggered when closing menu
	if (!bIsOpen)
	{
		ShutDownMenuAnchor->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UMainMenu::RotateTechCircle(const float InDeltaTime)
{
	float CurrentRotateAngle;

	// Rotate Circle 1 
	TechCircle->GetScalarParameterValue(TEXT("RotateAngle1"), CurrentRotateAngle);
	CurrentRotateAngle += (InDeltaTime / 15.0f);
	TechCircle->SetScalarParameterValue(TEXT("RotateAngle1"), CurrentRotateAngle);

	// Rotate Circle 2
	TechCircle->GetScalarParameterValue(TEXT("RotateAngle2"), CurrentRotateAngle);
	CurrentRotateAngle -= (InDeltaTime / 8.0f);
	TechCircle->SetScalarParameterValue(TEXT("RotateAngle2"), CurrentRotateAngle);

	// Rotate Circle 3
	TechCircle->GetScalarParameterValue(TEXT("RotateAngle3"), CurrentRotateAngle);
	CurrentRotateAngle += (InDeltaTime / 7.0f);
	TechCircle->SetScalarParameterValue(TEXT("RotateAngle3"), CurrentRotateAngle);
}

void UMainMenu::SetScreenElementColor(const int32 Index)
{
	const FLinearColor Color = ThemeColor[Index];

	BarImage_1->SetBrushTintColor(Color);
	BarImage_2->SetBrushTintColor(Color);
	BarImage_3->SetBrushTintColor(Color);

	PlayBtn->WidgetStyle.Normal.TintColor = Color;
	PlayBtn->WidgetStyle.Hovered.TintColor = Color;
	PlayBtn->WidgetStyle.Pressed.TintColor = Color;
	InventoryBtn->WidgetStyle.Normal.TintColor = Color;
	InventoryBtn->WidgetStyle.Hovered.TintColor = Color;
	InventoryBtn->WidgetStyle.Pressed.TintColor = Color;
}

void UMainMenu::OnSettingClick()
{
	// Switch to Setting Widget
	const ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->ChangeWidget.Execute(3);
}

void UMainMenu::OnInventoryClick()
{
	InventoryMenuAnchor->Open(true);
}

void UMainMenu::OnPlayClick()
{
	// Save player index
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));
	SaveGame->PlayerIndex = BtnScrollBox->PlayerIndex;
	UGameplayStatics::SaveGameToSlot(SaveGame, "Data", 0);
	
	// Switch to Level Menu
	const ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->ChangeWidget.Execute(1);
}

void UMainMenu::ShowSkillImage(int32 PlayerIndex)
{
	// Blue
	if (PlayerIndex == 0)
	{
		ThunderSkill->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	// Red
	if (PlayerIndex == 1)
	{
		HeartSkill->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	// Green
	if (PlayerIndex == 2)
	{
		ShieldSkill->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UMainMenu::HideSkillImage(int32 PlayerIndex)
{
	// Blue
	if (PlayerIndex == 0)
	{
		ThunderSkill->SetVisibility(ESlateVisibility::Collapsed);
	}

	// Red
	if (PlayerIndex == 1)
	{
		HeartSkill->SetVisibility(ESlateVisibility::Collapsed);
	}

	// Green
	if (PlayerIndex == 2)
	{
		ShieldSkill->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainMenu::OnRightBtnClick()
{
	if (BtnScrollBox->PlayerIndex == 2)
	{	
		return;
	}
	else
	{
		if (!BtnScrollBox->bSnapping)
		{
			// Change skill icon image
			HideSkillImage(BtnScrollBox->PlayerIndex);

			BtnScrollBox->PlayerIndex += 1;
			BtnScrollBox->bSnapping = true;
			BtnScrollBox->ScrollGridLocation += BtnScrollBox->ButtonSize;

			// Change skill icon image
			ShowSkillImage(BtnScrollBox->PlayerIndex);

			// Change screen element color
			SetScreenElementColor(BtnScrollBox->PlayerIndex);

			// Change player data text
			SetPlayerDataText(BtnScrollBox->PlayerIndex);

			PlayScrollSound();
		}
	}
}

void UMainMenu::OnLeftBtnClick()
{
	if (BtnScrollBox->PlayerIndex == 0)
	{
		return;
	}
	else
	{
		if (!BtnScrollBox->bSnapping)
		{
			if (BtnScrollBox->PlayerIndex == 2)
			{
				BtnScrollBox->SetScrollOffset(BtnScrollBox->PlayerIndex * BtnScrollBox->ButtonSize);
			}

			// Change skill icon image
			HideSkillImage(BtnScrollBox->PlayerIndex);
			
			BtnScrollBox->PlayerIndex -= 1;
			BtnScrollBox->bSnapping = true;
			BtnScrollBox->ScrollGridLocation -= BtnScrollBox->ButtonSize;

			// Change skill icon image
			ShowSkillImage(BtnScrollBox->PlayerIndex);

			// Change screen element color
			SetScreenElementColor(BtnScrollBox->PlayerIndex);

			// Change player data text
			SetPlayerDataText(BtnScrollBox->PlayerIndex);

			PlayScrollSound();
		}
	}
}