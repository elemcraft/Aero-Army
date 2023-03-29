// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingWidget.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGameMode.h"
#include "Components/Button.h"


void USettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackBtn)
	{
		BackBtn->OnClicked.AddDynamic(this, &USettingWidget::OnBackBtnClick);
	}
}

void USettingWidget::OnBackBtnClick()
{
	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->ChangeWidget.Execute(0);
}