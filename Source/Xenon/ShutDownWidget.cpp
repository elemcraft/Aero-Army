// Fill out your copyright notice in the Description page of Project Settings.


#include "ShutDownWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UShutDownWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (QuitBtn)
	{
		QuitBtn->OnClicked.AddDynamic(this, &UShutDownWidget::OnQuitClick);
	}
}

void UShutDownWidget::OnQuitClick()
{
	// Quit game
	UWorld* World = GetWorld();
	if (World)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("Quit"));
	}
}
