// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "SuperBallyBallGameMode.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum class EPlayState
{
	EPlaying,
	ELost,
	EWon
};

/**
 * 
 */
UCLASS()
class SUPERBALLYBALL_API ASuperBallyBallGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	// Sets default values for this game mode's properties
	ASuperBallyBallGameMode();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Return the time remaining */
	UFUNCTION(BlueprintPure, Category = "Time")
	FORCEINLINE float GetTimeRemaining() const { return TimeRemaining; }

	/** Return the game's current state */
	UFUNCTION(BlueprintPure, Category = "Game State")
	FORCEINLINE EPlayState GetCurrentState() const { return CurrentState; }

protected:
	/** The widget class for the HUD screen */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	/** The instance of the HUD */
	UPROPERTY()
	class UUserWidget* CurrentWidget;

private:
	/** Returns true if the ball has fallen below the level or thrown too far outside it, false otherwise */
	bool IsBallOutsideLevelBounds(class ABallPawn* BallPawn, class ALevelContainer* LevelContainer);

	/** Returns true if the ball is inside the Goal's static mesh, false otherwise */
	bool HasBallPassedThroughGoal(class ABallPawn* BallPawn, class AGoal* Goal);

	/** Set a new playing state and handle the consequence */
	void SetCurrentState(EPlayState NewState);

private:
	/** The number of seconds the player has left to complete the level */
	UPROPERTY(EditAnywhere)
	float TimeRemaining;

	/** Keeps track of the game's current state */
	EPlayState CurrentState;

	/** Protect against issue where 5 seconds are taken off TimeRemaining twice when falling below level once */
	bool bFellBelowLevel;
};
