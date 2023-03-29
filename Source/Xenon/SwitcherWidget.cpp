// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitcherWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "ChapterMenu.h"
#include "Components/AudioComponent.h"

//#include "Kismet/KismetSystemLibrary.h"

void USwitcherWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UChapterMenu* Widget = Cast<UChapterMenu>(LobbyWidgetSwitcher->GetWidgetAtIndex(4));
	Widget->SwitcherWidgetRef = this;

	// Play Door Animation
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USwitcherWidget::PlayDoorAnimation);
	GetWorld()->GetTimerManager().SetTimer(AnimDelayTimer, TimerDelegate, 0.5f, false);

	ShowAdAttempt = 0;
}

void USwitcherWidget::ToBattleLevel()
{
	// Destroy Ad Timer
	if (AdTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(AdTimer);
	}

	FWidgetAnimationDynamicEvent AnimationFinished;
	AnimationFinished.BindDynamic(this, &USwitcherWidget::LevelTransition);
	BindToAnimationFinished(DoorAnimation, AnimationFinished);
	PlayAnimation(DoorAnimation, 0.0f, 1, EUMGSequencePlayMode::Reverse, 2.0f);

	// Sound Effect
	UAudioComponent* SoundEffect = UGameplayStatics::CreateSound2D(GetWorld(), DoorCloseSound);
	SoundEffect->Play();
}

void USwitcherWidget::LevelTransition()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, TEXT("BattleLevel"));
	}
}

void USwitcherWidget::PlayDoorAnimation()
{
	FWidgetAnimationDynamicEvent DisplayAdvertisement;
	DisplayAdvertisement.BindDynamic(this, &USwitcherWidget::ShowAd);
	BindToAnimationFinished(DoorAnimation, DisplayAdvertisement);

	// Door animation sound effect
	UAudioComponent* SoundEffect = UGameplayStatics::CreateSound2D(GetWorld(), DoorOpenSound);
	if (SoundEffect)
	{
		SoundEffect->Play(); 
	}

	// Play animation
	PlayAnimation(DoorAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 2.0f);

	// Destroy delay timer
	GetWorld()->GetTimerManager().ClearTimer(AnimDelayTimer);
}

void USwitcherWidget::ShowAd()
{
	if (UKismetSystemLibrary::IsInterstitialAdAvailable())
	{
		UKismetSystemLibrary::ShowInterstitialAd();
	}
	else
	{
		if (ShowAdAttempt < 3)
		{
			FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USwitcherWidget::ShowAd);
			GetWorld()->GetTimerManager().SetTimer(AdTimer, TimerDelegate, 1.0f, false);

			ShowAdAttempt += 1;
		}
	}

	UnbindAllFromAnimationFinished(DoorAnimation);
}
