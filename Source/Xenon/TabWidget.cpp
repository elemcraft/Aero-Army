// Fill out your copyright notice in the Description page of Project Settings.


#include "TabWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGameMode.h"
#include "CustomSaveGame.h"
#include "RefineryWidget.h"

#include "SkillSystemWidget.h"


void UTabWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ShopCheckBox->OnCheckStateChanged.AddDynamic(this, &UTabWidget::OnShopCheck);
	MainMenuCheckBox->OnCheckStateChanged.AddDynamic(this, &UTabWidget::OnMainMenuCheck);
	RefineryCheckBox->OnCheckStateChanged.AddDynamic(this, &UTabWidget::OnRefineryCheck);

	DisableTextColor = FSlateColor(FLinearColor(0.236f, 0.236f, 0.236f, 1.0f));
	ActiveTextColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	ShopText->SetColorAndOpacity(DisableTextColor);
	RefineryText->SetColorAndOpacity(DisableTextColor);

	// Set Main Menu to be the initial widget
	TabSwitcher->SetActiveWidgetIndex(1);
	MainMenuCheckBox->SetCheckedState(ECheckBoxState::Checked);

	// Bind delegate
	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->ChangeTab.BindDynamic(this, &UTabWidget::SwitchTab);

	// Bind skill system upgrade delegate
	USkillSystemWidget* SkillSystemWidget = Cast<USkillSystemWidget>(GameMode->GetWidgetDelegate.Execute(2));
	SkillSystemWidget->SetItemBar.BindDynamic(this, &UTabWidget::SetupItemBar);

	// Bind refinery delegate
	URefineryWidget* Widget = Cast<URefineryWidget>(TabSwitcher->GetWidgetAtIndex(2));
	Widget->SetItemBarText.BindDynamic(this, &UTabWidget::SetupItemBar);

	SetupItemBar();
}

void UTabWidget::SetupItemBar()
{
	UCustomSaveGame* SaveGame = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot("Data", 0));

	GasCanText->SetText(ConvertToSIPrefix(SaveGame->ItemData[0]));
	BlueGasCanText->SetText(ConvertToSIPrefix(SaveGame->ItemData[1]));
	RedGasCanText->SetText(ConvertToSIPrefix(SaveGame->ItemData[2]));
	GreenGasCanText->SetText(ConvertToSIPrefix(SaveGame->ItemData[3]));
}

FText UTabWidget::ConvertToSIPrefix(const int32 InNumber)
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

void UTabWidget::OnShopCheck(bool bIsChecked)
{
	// Prevent spamming
	if (TabSwitcher->ActiveWidgetIndex == 0)
	{
		ShopCheckBox->SetCheckedState(ECheckBoxState::Checked);
		return;
	}

	MainMenuCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	MainMenuText->SetColorAndOpacity(DisableTextColor);
	RefineryCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	RefineryText->SetColorAndOpacity(DisableTextColor);

	// Switch to Shop Widget
	ShopText->SetColorAndOpacity(ActiveTextColor);
	TabSwitcher->SetActiveWidgetIndex(0);
}

void UTabWidget::OnMainMenuCheck(bool bIsChecked)
{
	// Prevent spamming
	if (TabSwitcher->ActiveWidgetIndex == 1)
	{
		MainMenuCheckBox->SetCheckedState(ECheckBoxState::Checked);
		return;
	}

	ShopCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	ShopText->SetColorAndOpacity(DisableTextColor);
	RefineryCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	RefineryText->SetColorAndOpacity(DisableTextColor);

	// Switch to Main Menu
	MainMenuText->SetColorAndOpacity(ActiveTextColor);
	TabSwitcher->SetActiveWidgetIndex(1);
}

void UTabWidget::OnRefineryCheck(bool bIsChecked)
{
	// Prevent spamming
	if (TabSwitcher->ActiveWidgetIndex == 2)
	{
		RefineryCheckBox->SetCheckedState(ECheckBoxState::Checked);
		return;
	}

	ShopCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	ShopText->SetColorAndOpacity(DisableTextColor);
	MainMenuCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	MainMenuText->SetColorAndOpacity(DisableTextColor);

	// Switch to Refinery
	RefineryText->SetColorAndOpacity(ActiveTextColor);
	TabSwitcher->SetActiveWidgetIndex(2);
}

void UTabWidget::SwitchTab(int32 Index)
{
	if (Index == 0)
	{
		ShopCheckBox->SetCheckedState(ECheckBoxState::Checked);
		OnShopCheck(true);
		return;
	}
	if (Index == 1)
	{
		MainMenuCheckBox->SetCheckedState(ECheckBoxState::Checked);
		OnMainMenuCheck(true);
		return;
	}
	if (Index == 2)
	{
		RefineryCheckBox->SetCheckedState(ECheckBoxState::Checked);
		OnRefineryCheck(true);
		return;
	}
}


