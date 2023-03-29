// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceshipBtn.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGameMode.h"
#include "LobbyController.h"

USpaceshipBtn::USpaceshipBtn()
{
	WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;
	WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;
	WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;
	WidgetStyle.Disabled.DrawAs = ESlateBrushDrawType::Image;

	ClickMethod = EButtonClickMethod::PreciseClick;
	TouchMethod = EButtonTouchMethod::PreciseTap;
	PressMethod = EButtonPressMethod::ButtonRelease;

	OnClicked.AddDynamic(this, &USpaceshipBtn::OnButtonClick);
}

void USpaceshipBtn::OnButtonClick()
{
	// Setup skill system widget (Execute this in NativeConstruct() in skill system will get wrong information)
	ALobbyController* Controller = Cast<ALobbyController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	Controller->SetupSkillSystemWidgetDelegate.Execute();

	// Switch to Skill System Widget
	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->ChangeWidget.Execute(2);

	// Bind Skill System Widget input
	Controller->SkillSystemInput(true);

	// Remember to clear action binding 
	// 1. On back button click
	// 2. On Android Back button click
}
