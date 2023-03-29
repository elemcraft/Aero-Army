// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonScrollBox.h"

UButtonScrollBox::UButtonScrollBox()
{
	WidgetStyle.TopShadowBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
	WidgetStyle.BottomShadowBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
	WidgetStyle.RightShadowBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
	WidgetStyle.LeftShadowBrush.DrawAs = ESlateBrushDrawType::NoDrawType;

	// Snap variable
	SwipeLength = 150.0f;
	ButtonSize = 650.0f;
	bSnapping = false;
}

void UButtonScrollBox::ScrollBoxSnap(const float DeltaTime)
{
	const float ScrollOffset = GetScrollOffset();

	// Avoid being stuck at edge
	if (ScrollOffset > GetScrollOffsetOfEnd() - 1.0f)
	{
		HideSkillImage.Execute(PlayerIndex);	// Change skill icon image
		PlayerIndex = 2;
		ShowSkillImage.Execute(PlayerIndex);	// Change skill icon image
		ChangeScreenElementColor.Execute(PlayerIndex);    // Change screen element color
		ChangePlayerDataText.Execute(PlayerIndex);    // Change player data text
		ScrollGridLocation = PlayerIndex * ButtonSize;
		bSnapping = false;
		return;
	}
	else if (ScrollOffset < 0)
	{
		HideSkillImage.Execute(PlayerIndex);	// Change skill icon image
		PlayerIndex = 0;
		ShowSkillImage.Execute(PlayerIndex);	// Change skill icon image
		ChangeScreenElementColor.Execute(PlayerIndex);    // Change screen element color
		ChangePlayerDataText.Execute(PlayerIndex);    // Change player data text
		ScrollGridLocation = 0;
		bSnapping = false;
		return;
	}

	// Scroll box snap
	const bool HandsOff = !HasMouseCapture();
	const bool NotOnSpot = (ScrollOffset != ScrollGridLocation);

	if (HandsOff && NotOnSpot)
	{
		const bool IsSwipe = FMath::Abs(ScrollOffset - ScrollGridLocation) > SwipeLength;
		if (IsSwipe && !bSnapping)
		{
			UE_LOG(LogTemp, Warning, TEXT("Set New Destination"));

			const int32 ScrollDirection = FMath::Sign(ScrollOffset - ScrollGridLocation);
			ScrollGridLocation += ScrollDirection * ButtonSize;
			HideSkillImage.Execute(PlayerIndex);	// Change skill icon image
			PlayerIndex += ScrollDirection;
			ShowSkillImage.Execute(PlayerIndex);	// Change skill icon image
			ChangeScreenElementColor.Execute(PlayerIndex);    // Change screen element color
			ChangePlayerDataText.Execute(PlayerIndex);    // Change player data text
		}

		EndInertialScrolling();

		// Three stage snapping
		if (FMath::Abs(ScrollOffset - ScrollGridLocation) > 330.0f)
		{
			SetScrollOffset(FMath::FInterpConstantTo(ScrollOffset, ScrollGridLocation, DeltaTime, SnapSpeed * 330.0f));
		}
		else if (FMath::Abs(ScrollOffset - ScrollGridLocation) < 100.0f)
		{
			SetScrollOffset(FMath::FInterpConstantTo(ScrollOffset, ScrollGridLocation, DeltaTime, SnapSpeed * 100.0f));
		}
		else
		{
			SetScrollOffset(FMath::FInterpTo(ScrollOffset, ScrollGridLocation, DeltaTime, SnapSpeed));
		}

		bSnapping = true;
	}
	else
	{
		bSnapping = false;
	}
}