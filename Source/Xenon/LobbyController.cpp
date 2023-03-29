// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyController.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGameMode.h"
#include "MainMenu.h"
#include "Components/MenuAnchor.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanel.h"
#include "SkillSystemWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "CustomSaveGame.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

ALobbyController::ALobbyController()
{
	
}

void ALobbyController::BeginPlay()
{
	Super::BeginPlay();

	// Disable virtual joystick in lobby
	ActivateTouchInterface(nullptr);

	// Test
	// SetInputMode(FInputModeGameAndUI());

	SetupSkillSystemWidgetDelegate.BindDynamic(this, &ALobbyController::SetupSkillSystemWidget);
}

void ALobbyController::SkillSystemInput(bool bEnable)
{
	if (bEnable)
	{
		// Pinch
		InputComponent->BindAction("PinchStart", IE_Pressed, this, &ALobbyController::OnPinchStart);
		InputComponent->BindAction("PinchEnd", IE_Released, this, &ALobbyController::OnPinchEnd);

		// Drag
		InputComponent->BindAction("Drag", IE_Pressed, this, &ALobbyController::OnTouchStart);
		InputComponent->BindAction("Drag", IE_Repeat, this, &ALobbyController::OnDrag);
		InputComponent->BindAction("Drag", IE_Released, this, &ALobbyController::OnDragEnd);

		// Input Variable
		bPinch = false;
		bDrag = false;
	}
	else
	{
		// Pinch
		InputComponent->RemoveActionBinding("PinchStart", IE_Pressed);
		InputComponent->RemoveActionBinding("PinchEnd", IE_Released);

		// Drag
		InputComponent->RemoveActionBinding("Drag", IE_Pressed);
		InputComponent->RemoveActionBinding("Drag", IE_Repeat);
		InputComponent->RemoveActionBinding("Drag", IE_Released);
	}
}

void ALobbyController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bPinch && !SkillSystemWidgetRef->UpgradeMenuAnchor->IsOpen())
	{
		HandlePinch(DeltaTime);
	}
}

void ALobbyController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("AndroidBack", IE_Pressed, this, &ALobbyController::OnBackClick);
	InputComponent->BindAction("AndroidBackTest", IE_Pressed, this, &ALobbyController::OnBackClick);
}

void ALobbyController::AddWidgetToViewport(UPARAM(Ref) const TSubclassOf<class UUserWidget>& WidgetClass)
{
	if (WidgetClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			UUserWidget* Widget = CreateWidget(World, WidgetClass);
			if (Widget)
			{
				Widget->AddToViewport();
			}
		}	
	}
}

void ALobbyController::SetupSkillSystemWidget()
{
	const ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	SkillSystemWidgetRef = Cast<USkillSystemWidget>(GameMode->LobbyWidgetSwitcher->GetWidgetAtIndex(2));

	// Set skill system item bar
	SkillSystemWidgetRef->UpdateSkillItemBar();

	// Restore SkillSystemWidget
	SkillSystemWidgetRef->SkillTreeCanvas->SetRenderTranslation(FVector2D::ZeroVector);
	SkillSystemWidgetRef->SkillTreeCanvas->SetRenderScale(FVector2D::UnitVector);

	// Canvas Local Size
	FGeometry Geo;
	UCanvasPanelSlot* Canvas = UWidgetLayoutLibrary::SlotAsCanvasSlot(SkillSystemWidgetRef->SkillTreeCanvas);
	SkillTreeCanvasSize = Canvas->GetSize();

	// ViewportSize
	int32 ViewportSizeX;
	int32 ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ViewportSize = FVector2D(ViewportSizeX, ViewportSizeY);

	// ViewportScale
	float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());

	// RealSize
	RealViewportSize = FVector2D(ViewportSizeX / ViewportScale, ViewportSizeY / ViewportScale);
}

void ALobbyController::OnBackClick()
{
	const ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	const bool bInTabWidget = GameMode->GetCurrentWidgetIndex.Execute() == 0;
	if (bInTabWidget)
	{
		const bool bInMainMenu = GameMode->InMainMenu.Execute();
		if (bInMainMenu)
		{
			// Toggle "Shut Down Widget" 
			UMainMenu* Widget = Cast<UMainMenu>(GameMode->TabSwitcher->GetActiveWidget());
			Widget->ShutDownMenuAnchor->ToggleOpen(true);

			return;
		}
		else
		{
			// TabSwitcher switch to Main Menu
			GameMode->ChangeTab.Execute(1);

			return;
		}
	}
	else
	{
		// Disable skill system input if leaving skill system widget
		const bool bLeaveSkillSystem = GameMode->GetCurrentWidgetIndex.Execute() == 2;
		if (bLeaveSkillSystem)
		{
			SkillSystemInput(false);
		}

		const bool  bLeaveChapterMenu = GameMode->GetCurrentWidgetIndex.Execute() == 4;
		if (bLeaveChapterMenu)
		{
			// Go to Level Menu
			GameMode->ChangeWidget.Execute(1);
			return;
		}

		// Back to Tab Widget
		GameMode->ChangeWidget.Execute(0);
	}
}

void ALobbyController::HandlePinch(const float InDeltaTime)
{
	bool bTemp;
	FVector2D TouchStartOne;
	FVector2D TouchStartTwo;
	GetInputTouchState(ETouchIndex::Touch1, TouchStartOne.X, TouchStartOne.Y, bTemp);
	GetInputTouchState(ETouchIndex::Touch2, TouchStartTwo.X, TouchStartTwo.Y, bTemp);
	const float CurrentFingerDistance = (TouchStartOne - TouchStartTwo).Size();
	UpdateCanvasScale(CurrentFingerDistance / FingerDistance, InDeltaTime);
	FingerDistance = CurrentFingerDistance;
}

void ALobbyController::UpdateCanvasScale(const float ScaleMultiplier, const float InDeltaTime)
{
	// Update widget scale
	FWidgetTransform WidgetTransform = SkillSystemWidgetRef->SkillTreeCanvas->RenderTransform;
	float Scale = WidgetTransform.Scale.X;
	Scale *= ScaleMultiplier;
	Scale = FMath::Clamp(Scale, 1.0f, 3.0f);
	WidgetTransform.Scale = FVector2D(Scale);

	// Adjust translation to fit the new scale
	WidgetTransform.Translation.X = WidgetTransform.Translation.X * ScaleMultiplier;
	WidgetTransform.Translation.Y = WidgetTransform.Translation.Y * ScaleMultiplier;

	// Set widget transform
	SkillSystemWidgetRef->SkillTreeCanvas->SetRenderTransform(WidgetTransform);
}

void ALobbyController::OnPinchStart()
{
	if (!bPinch)
	{
		bool bTemp;
		FVector2D TouchStartOne;
		FVector2D TouchStartTwo;
		GetInputTouchState(ETouchIndex::Touch1, TouchStartOne.X, TouchStartOne.Y, bTemp);
		GetInputTouchState(ETouchIndex::Touch2, TouchStartTwo.X, TouchStartTwo.Y, bTemp);
		FingerDistance = (TouchStartOne - TouchStartTwo).Size();

		bPinch = true;
	}
}

void ALobbyController::OnPinchEnd()
{
	bPinch = false;
}

void ALobbyController::OnTouchStart()
{
	if (!bPinch)
	{
		bool Temp;
		bDrag = true;
		GetInputTouchState(ETouchIndex::Touch1, TouchStartLocation.X, TouchStartLocation.Y, Temp);
	}
}

void ALobbyController::OnDrag()
{
	if (!bPinch)
	{
		bool bTemp;
		FVector2D CurrentLocation;
		GetInputTouchState(ETouchIndex::Touch1, CurrentLocation.X, CurrentLocation.Y, bTemp);
		if (bDrag && !SkillSystemWidgetRef->UpgradeMenuAnchor->IsOpen())
		{
			// DPI
			float DPI = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
			
			// Update widegt translation
			FWidgetTransform WidgetTransform = SkillSystemWidgetRef->SkillTreeCanvas->RenderTransform;
			WidgetTransform.Translation += (CurrentLocation - TouchStartLocation) / DPI;
			WidgetTransform.Translation.X = FMath::Clamp(WidgetTransform.Translation.X, -SkillTreeCanvasSize.X / 2.0f * WidgetTransform.Scale.X, SkillTreeCanvasSize.X / 2.0f * WidgetTransform.Scale.X);
			WidgetTransform.Translation.Y = FMath::Clamp(WidgetTransform.Translation.Y, -SkillTreeCanvasSize.Y / 2.0f * WidgetTransform.Scale.Y, SkillTreeCanvasSize.Y / 2.0f * WidgetTransform.Scale.Y);

			// Set widget translation
			SkillSystemWidgetRef->SkillTreeCanvas->SetRenderTranslation(WidgetTransform.Translation);
		}
		else
		{
			bDrag = true;
		}

		// Update touch location
		TouchStartLocation = CurrentLocation;
		return;
	}
}

void ALobbyController::OnDragEnd()
{
	bDrag = false;
}
