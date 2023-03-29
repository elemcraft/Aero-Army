// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyController.generated.h"

/**
 * 
 */

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE(FSetupSkillSystemWidget);

UCLASS()
class XENON_API ALobbyController : public APlayerController
{
	GENERATED_BODY()

private:
	ALobbyController();

public:
	virtual void PlayerTick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void SkillSystemInput(bool bEnable);

	UPROPERTY(BlueprintReadOnly)
	class USkillSystemWidget* SkillSystemWidgetRef;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Delegate")
	FSetupSkillSystemWidget SetupSkillSystemWidgetDelegate;

	UFUNCTION(BlueprintCallable, Category = "Delegate Function")
	void SetupSkillSystemWidget();

	UPROPERTY(BlueprintReadOnly , Category = "Input")
	FVector2D SkillTreeCanvasSize;

	UPROPERTY(BlueprintReadOnly, Category = "Input")
	FVector2D RealViewportSize;

	UFUNCTION(BlueprintCallable, Category = "Android Button")
	void OnBackClick();

	UFUNCTION(BlueprintCallable, Category = "Pinch")
	void OnPinchStart();

	UFUNCTION(BlueprintCallable, Category = "Pinch")
	void OnPinchEnd();

	UPROPERTY(BlueprintReadWrite, Category = "Pinch")
	bool bPinch;

	UPROPERTY(BlueprintReadWrite, Category = "Pinch")
	float FingerDistance;

	UFUNCTION(BlueprintCallable, Category = "Pinch")
	void HandlePinch(const float InDeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Pinch")
	void UpdateCanvasScale(const float ScaleMultiplier, const float InDeltaTime);

public: 
	UPROPERTY(BlueprintReadWrite, Category = "Drag")
	FVector2D TouchStartLocation;

	UFUNCTION(BlueprintCallable, Category = "Drag")
	void OnTouchStart();

	UFUNCTION(BlueprintCallable, Category = "Drag")
	void OnDrag();

	UFUNCTION(BlueprintCallable, Category = "Drag")
	void OnDragEnd();

	UPROPERTY(BlueprintReadWrite, Category = "Drag")
	bool bDrag;

protected:
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void AddWidgetToViewport(UPARAM(Ref) const TSubclassOf<class UUserWidget>& WidgetClass);
};
