// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSystemWidget.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGameMode.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "LobbyController.h"
#include "Components/CheckBox.h"
#include "Components/MenuAnchor.h"
#include "UpgradeMenu.h"
#include "CustomSaveGame.h"
#include "Components/AudioComponent.h"
#include "Components/TextBlock.h"


void USkillSystemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackBtn)
	{
		BackBtn->OnClicked.AddDynamic(this, &USkillSystemWidget::OnBackBtnClick);
	}

	// Get background star material
	BGStar = StarBackgroundImage->GetDynamicMaterial();

	// Upgrade Check Box
	UpgradeCheckBox.Add(BlueHealthUpgrade);
	UpgradeCheckBox.Add(BlueAttackUpgrade);
	UpgradeCheckBox.Add(BlueAttackSpeedUpgrade);
	UpgradeCheckBox.Add(BlueSkillUpgrade);
	UpgradeCheckBox.Add(RedHealthUpgrade);
	UpgradeCheckBox.Add(RedAttackUpgrade);
	UpgradeCheckBox.Add(RedAttackSpeedUpgrade);
	UpgradeCheckBox.Add(RedSkillUpgrade);
	UpgradeCheckBox.Add(GreenHealthUpgrade);
	UpgradeCheckBox.Add(GreenAttackUpgrade);
	UpgradeCheckBox.Add(GreenAttackSpeedUpgrade);
	UpgradeCheckBox.Add(GreenSkillUpgrade);
	UpgradeCheckBox.Add(MaximumStaminaUpgrade);
	UpgradeCheckBox.Add(GasCanDropRateUpgrade);
	UpgradeCheckBox.Add(RefinerCapacityUpgrade);
	UpgradeCheckBox.Add(RefinerSpeedUpgrade);

	auto BindCheckBoxFunction = [&](void) -> void
	{
		// Blue
		BlueHealthUpgrade->OnCheckStateChanged.AddDynamic(this, &USkillSystemWidget::BlueHealthCheck);
		BlueAttackUpgrade->OnCheckStateChanged.AddDynamic(this, &USkillSystemWidget::BlueAttackCheck);
		BlueAttackSpeedUpgrade->OnCheckStateChanged.AddDynamic(this, &USkillSystemWidget::BlueAttackSpeedCheck);
		BlueSkillUpgrade->OnCheckStateChanged.AddDynamic(this, &USkillSystemWidget::BlueSkillCheck);

		// Red
		RedHealthUpgrade->OnCheckStateChanged.AddDynamic(this, &USkillSystemWidget::RedHealthCheck);
		RedAttackUpgrade->OnCheckStateChanged.AddDynamic(this, &USkillSystemWidget::RedAttackCheck);
		RedAttackSpeedUpgrade->OnCheckStateChanged.AddDynamic(this, &USkillSystemWidget::RedAttackSpeedCheck);
		RedSkillUpgrade->OnCheckStateChanged.AddDynamic(this, &USkillSystemWidget::RedSkillCheck);

		// Green
		GreenHealthUpgrade->OnCheckStateChanged.AddDynamic(this, &USkillSystemWidget::GreenHealthCheck);
		GreenAttackUpgrade->OnCheckStateChanged.AddDynamic(this, &USkillSystemWidget::GreenAttackCheck);
		GreenAttackSpeedUpgrade->OnCheckStateChanged.AddDynamic(this, &USkillSystemWidget::GreenAttackSpeedCheck);
		GreenSkillUpgrade->OnCheckStateChanged.AddDynamic(this, &USkillSystemWidget::GreenSkillCheck);

		// Global
		MaximumStaminaUpgrade->OnCheckStateChanged.AddDynamic(this, &USkillSystemWidget::MaximumStaminaCheck);
		GasCanDropRateUpgrade->OnCheckStateChanged.AddDynamic(this, &USkillSystemWidget::GasCanDropRateCheck);
		RefinerCapacityUpgrade->OnCheckStateChanged.AddDynamic(this, &USkillSystemWidget::RefinerCapacityCheck);
		RefinerSpeedUpgrade->OnCheckStateChanged.AddDynamic(this, &USkillSystemWidget::RefinerSpeedCheck);
	};
	BindCheckBoxFunction();

	// Upgrade Menu Anchor
	UpgradeMenuAnchor->OnMenuOpenChanged.AddDynamic(this, &USkillSystemWidget::CloseUpgradeMenu);
	UpgradeMenuAnchor->OnGetUserMenuContentEvent.BindDynamic(this, &USkillSystemWidget::ShowUpgradeMenu);

	// Setup skill tree flipbook
	FTimerHandle SkillTreeUpdateTimer;
	GetWorld()->GetTimerManager().SetTimer(SkillTreeUpdateTimer, this, &USkillSystemWidget::UpdateSkillTree, 1.0f / 24.0f, true);
}

void USkillSystemWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	RotateBackground(InDeltaTime);
}

void USkillSystemWidget::OnBackBtnClick()
{
	// Disable skill system input
	ALobbyController* Controller = Cast<ALobbyController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	Controller->SkillSystemInput(false);

	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->ChangeWidget.Execute(0);
}

void USkillSystemWidget::RotateBackground(const float InDeltaTime)
{
	float CurrentAngle;
	BGStar->GetScalarParameterValue(TEXT("RotateAngle"), CurrentAngle);

	CurrentAngle += (InDeltaTime / 150.0f);

	BGStar->SetScalarParameterValue(TEXT("RotateAngle"), CurrentAngle);
}

void USkillSystemWidget::CloseUpgradeMenu(bool bIsOpen)
{
	// Close upgrade menu
	if (!bIsOpen)
	{
		UpgradeCheckBox[UpgradeMenuIndex]->SetIsChecked(false);
	}
}

UUserWidget* USkillSystemWidget::ShowUpgradeMenu()
{
	UUpgradeMenu* Widget = CreateWidget<UUpgradeMenu>(GetWorld(), UpgradeMenus[UpgradeMenuIndex]);
	Widget->SkillWidgetRef = this;
	
	return Widget;
}

void USkillSystemWidget::PlayUpgradeSoundEffect(const bool bUpgradeSuccess)
{
	if (bUpgradeSuccess)
	{
		UAudioComponent* SuccessSoundEffect = UGameplayStatics::CreateSound2D(GetWorld(), UpgradeSuccessSound);
		if (SuccessSoundEffect)
		{
			SuccessSoundEffect->Play();
		}
	}
	else
	{
		UAudioComponent* FailSoundEffect = UGameplayStatics::CreateSound2D(GetWorld(), UpgradeFailSound);
		if (FailSoundEffect)
		{
			FailSoundEffect->Play();
		}
	}
}

void USkillSystemWidget::UpdateSkillItemBar()
{
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));

	GasCanText->SetText(ConvertToSIPrefix(SaveGame->ItemData[0]));
	BlueGasCanText->SetText(ConvertToSIPrefix(SaveGame->ItemData[1]));
	RedGasCanText->SetText(ConvertToSIPrefix(SaveGame->ItemData[2]));
	GreenGasCanText->SetText(ConvertToSIPrefix(SaveGame->ItemData[3]));
}

FText USkillSystemWidget::ConvertToSIPrefix(const int32 InNumber)
{
	if (InNumber < pow(10, 3))
	{
		return FText::FromString(FString::FromInt(InNumber));
	}
	else if (InNumber >= pow(10, 3) && InNumber < pow(10, 6))
	{
		return FText::FromString(FString::FromInt(InNumber / pow(10, 3)) + "k");
	}
	else if (InNumber >= pow(10, 6) && InNumber < pow(10, 9))
	{
		return FText::FromString(FString::FromInt(InNumber / pow(10, 6)) + "M");
	}
	else if (InNumber < pow(10, 9) && InNumber < pow(10, 12))
	{
		return FText::FromString(FString::FromInt(InNumber / pow(10, 9)) + "G");
	}

	FString Overflow = "+999G";
	return FText::FromString(Overflow);
}

void USkillSystemWidget::OnUpgradeCheck()
{
	PlayAnimation(PopUpAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.35f, false);
	UpgradeMenuAnchor->ToggleOpen(true);
}

void USkillSystemWidget::BlueHealthCheck(bool bIsChecked)
{
	if (bIsChecked)
	{
		UpgradeMenuIndex = 0;
		OnUpgradeCheck();
	}
}

void USkillSystemWidget::BlueAttackCheck(bool bIsChecked)
{
	if (bIsChecked)
	{
		UpgradeMenuIndex = 1;
		OnUpgradeCheck();
	}
}

void USkillSystemWidget::BlueAttackSpeedCheck(bool bIsChecked)
{
	if (bIsChecked)
	{
		UpgradeMenuIndex = 2;
		OnUpgradeCheck();
	}
}

void USkillSystemWidget::BlueSkillCheck(bool bIsChecked)
{
	if (bIsChecked)
	{
		UpgradeMenuIndex = 3;
		OnUpgradeCheck();
	}
}

void USkillSystemWidget::RedHealthCheck(bool bIsChecked)
{
	if (bIsChecked)
	{
		UpgradeMenuIndex = 4;
		OnUpgradeCheck();
	}
}

void USkillSystemWidget::RedAttackCheck(bool bIsChecked)
{
	if (bIsChecked)
	{
		UpgradeMenuIndex = 5;
		OnUpgradeCheck();
	}
}

void USkillSystemWidget::RedAttackSpeedCheck(bool bIsChecked)
{
	if (bIsChecked)
	{
		UpgradeMenuIndex = 6;
		OnUpgradeCheck();
	}
}

void USkillSystemWidget::RedSkillCheck(bool bIsChecked)
{
	if (bIsChecked)
	{
		UpgradeMenuIndex = 7;
		OnUpgradeCheck();
	}
}

void USkillSystemWidget::GreenHealthCheck(bool bIsChecked)
{
	if (bIsChecked)
	{
		UpgradeMenuIndex = 8;
		OnUpgradeCheck();
	}
}

void USkillSystemWidget::GreenAttackCheck(bool bIsChecked)
{
	if (bIsChecked)
	{
		UpgradeMenuIndex = 9;
		OnUpgradeCheck();
	}
}

void USkillSystemWidget::GreenAttackSpeedCheck(bool bIsChecked)
{
	if (bIsChecked)
	{
		UpgradeMenuIndex = 10;
		OnUpgradeCheck();
	}
}

void USkillSystemWidget::GreenSkillCheck(bool bIsChecked)
{
	if (bIsChecked)
	{
		UpgradeMenuIndex = 11;
		OnUpgradeCheck();
	}
}

void USkillSystemWidget::MaximumStaminaCheck(bool bIsChecked)
{
	if (bIsChecked)
	{
		UpgradeMenuIndex = 12;
		OnUpgradeCheck();
	}
}

void USkillSystemWidget::GasCanDropRateCheck(bool bIsChecked)
{
	if (bIsChecked)
	{
		UpgradeMenuIndex = 13;
		OnUpgradeCheck();
	}
}

void USkillSystemWidget::RefinerCapacityCheck(bool bIsChecked)
{
	if (bIsChecked)
	{
		UpgradeMenuIndex = 14;
		OnUpgradeCheck();
	}
}

void USkillSystemWidget::RefinerSpeedCheck(bool bIsChecked)
{
	if (bIsChecked)
	{
		UpgradeMenuIndex = 15;
		OnUpgradeCheck();
	}
}
