// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawnBase.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateHealthBar, float, InHealth);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateGasCanCount, int32, InCount);

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE(FTakeDamageAnimation);

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_OneParam(FEnableEnemySpawner, bool, bEnable);

UCLASS()
class XENON_API APlayerPawnBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawnBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* PlayerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* Muzzle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	class UParticleSystem* ExplodeParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	class USoundBase* PlayerEnterSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	class USoundBase* PlayerLeaveSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	class USoundBase* PlayerTakeDamageSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	class USoundBase* PlayerAttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	class USoundBase* PlayerSkillSound;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	class APlayerController* PC;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	FVector2D TouchStartPosition;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void TouchStartFunction(const ETouchIndex::Type FingerIndex, const FVector Location);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void TouchRepeatFunction(const ETouchIndex::Type FingerIndex, const FVector Location);

	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	FTimerHandle SkillTimer;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	virtual void CastSkill();

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	FTimerHandle AttackTimer;

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	float AttackValue;

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	float AttackSpeed;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<class AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<class UCameraShakeBase> ExplodeCamShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	class USoundBase* ExplodeSound;

	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float MaxHealth;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void ReceiveDamage(const float Damage);

	UPROPERTY(BlueprintReadOnly)
	bool bReceiveDamage;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void PlayerDie();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collide")
	float CollideDamage;

public:
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	FTakeDamageAnimation TakeDamageAnimation;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	FUpdateHealthBar UpdateHealthBar;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	FUpdateGasCanCount UpdateGasCanCount;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
	int32 GasCanCount;

public:
	UFUNCTION(BlueprintCallable, Category = "Entrance Animation")
	void PlayEntranceAnimation();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Entrance Animation")
	class UTimelineComponent* EntranceAnimTimeline;

	UPROPERTY(EditAnywhere, Category = "Entrance Animation")
	class UCurveFloat* EntranceAnimCurve;

	UFUNCTION(BlueprintCallable, Category = "Entrance Animation")
	void EntranceAnimationUpdate(const float Value);

	UFUNCTION(BlueprintCallable, Category = "Entrance Animation")
	void EntranceAnimationFinish();

public:
	UFUNCTION(BlueprintCallable, Category = "Exit Animation")
	void MissionSuccess();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Exit Animation")
	class UTimelineComponent* ExitAnimTimeline;

	UPROPERTY(EditAnywhere, Category = "Exit Animation")
	class UCurveFloat* ExitAnimCurve;

	UFUNCTION(BlueprintCallable, Category = "Exit Animation")
	void ExitAnimationUpdate(const float Value);

	UFUNCTION(BlueprintCallable, Category = "Exit Aninmation")
	void ExitAnimationFinish();
	
	UPROPERTY(BlueprintReadWrite, Category = "Exit Animation")
	FVector EndLocation;

public:
	UPROPERTY(BlueprintReadWrite)
	FEnableEnemySpawner EnableEnemySpawner; 

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> Reward;

protected:
	UFUNCTION(BlueprintCallable)
	void SaveRewardAndProgress(const bool bMissionSuccess);
};
