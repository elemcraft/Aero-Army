// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopWidget.h"
#include "Components/Button.h"
#include "Components/MenuAnchor.h"

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button0->OnClicked.AddDynamic(this, &UShopWidget::OnFormationClick);
	Button1->OnClicked.AddDynamic(this, &UShopWidget::OnTurretClick);
	Button2->OnClicked.AddDynamic(this, &UShopWidget::OnLaserClick);
	Button3->OnClicked.AddDynamic(this, &UShopWidget::OnLaserTurretClick);
	Button4->OnClicked.AddDynamic(this, &UShopWidget::OnMegaClick);
	Button5->OnClicked.AddDynamic(this, &UShopWidget::OnAggressiveMegaClick);
	Button6->OnClicked.AddDynamic(this, &UShopWidget::OnSineAsteroidClick);
	Button7->OnClicked.AddDynamic(this, &UShopWidget::OnSniperClick);

	DetailMenuAnchor->OnGetUserMenuContentEvent.BindDynamic(this, &UShopWidget::SelectDetailMenu);
	DetailMenuAnchor->OnMenuOpenChanged.AddDynamic(this, &UShopWidget::CloseDetailMenu);
}

void UShopWidget::ShowDetailMenu()
{
	DetailMenuAnchor->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(BlurAnimation);

	DetailMenuAnchor->Open(true);
}

UUserWidget* UShopWidget::SelectDetailMenu()
{
	UUserWidget* Widget = CreateWidget(GetWorld(), DetailMenus[DetailMenuIndex]);
	return Widget;
}

void UShopWidget::CloseDetailMenu(bool bIsOpen)
{
	// Only triggered when closing menu
	if (!bIsOpen)
	{
		DetailMenuAnchor->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UShopWidget::OnFormationClick()
{
	DetailMenuIndex = 0;
	ShowDetailMenu();
}

void UShopWidget::OnTurretClick()
{
	DetailMenuIndex = 1;
	ShowDetailMenu();
}

void UShopWidget::OnLaserClick()
{
	DetailMenuIndex = 2;
	ShowDetailMenu();
}

void UShopWidget::OnLaserTurretClick()
{
	DetailMenuIndex = 3;
	ShowDetailMenu();
}

void UShopWidget::OnMegaClick()
{
	DetailMenuIndex = 4;
	ShowDetailMenu();
}

void UShopWidget::OnAggressiveMegaClick()
{
	DetailMenuIndex = 5;
	ShowDetailMenu();
}

void UShopWidget::OnSineAsteroidClick()
{
	DetailMenuIndex = 6;
	ShowDetailMenu();
}

void UShopWidget::OnSniperClick()
{
	DetailMenuIndex = 7;
	ShowDetailMenu();
}
