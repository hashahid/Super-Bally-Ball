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

	/** Return the total time */
	UFUNCTION(BlueprintPure, Category = "Time")
	FORCEINLINE float GetTotalTime() const { return TotalTime; }

	/** Return the time remaining */
	UFUNCTION(BlueprintPure, Category = "Time")
	FORCEINLINE float GetTimeRemaining() const { return TimeRemaining; }

	/** Sets the time in seconds needed to complete a level */
	UFUNCTION(BlueprintCallable, Category = "Time")
	void SetTimeForLevel(float Time);

	/** Return the game's current state */
	UFUNCTION(BlueprintPure, Category = "Game State")
	FORCEINLINE EPlayState GetCurrentState() const { return CurrentState; }

	/** Return the SaveGame object */
	UFUNCTION(BlueprintPure, Category = "High Score")
	FORCEINLINE class USBBSaveGame* GetSaveGame() const { return SaveGame; }

protected:
	/** The widget class for the HUD screen */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	/** The instance of the HUD */
	UPROPERTY()
	class UUserWidget* CurrentWidget;

private:
	/** Returns true if the ball has fallen below the level or thrown too far outside it, false otherwise */
	bool IsBallOutsideLevelBounds(class ABallPawn* BallPawn, class ACenterMarker* LevelCenter);

	/** Returns true if the ball is inside the Goal's static mesh, false otherwise */
	bool HasBallPassedThroughGoal(class ABallPawn* BallPawn, class AGoal* Goal);

	/** Set a new playing state and handle the consequence */
	void SetCurrentState(EPlayState NewState);

	/** Pause or unpause the game depending on value of parameter boolean. Mainly used to pause after winning */
	void ASuperBallyBallGameMode::SetGamePaused(bool bIsPaused);

private:
	/** The main ball pawn. The game mode keeps a reference so it can capture the pawn once and use it for operations */
	class ABallPawn* BallPawn;

	/** The level's center. The game mode keeps a reference so it can capture it once and use it for boundary tests */
	class ACenterMarker* LevelCenter;

	/** The level's goal. The game mode keeps a reference so it can capture it once and use it for win condition tests */
	class AGoal* Goal;

	/** The number of seconds the player has to complete the level */
	UPROPERTY(EditAnywhere)
	float TotalTime;

	/** The number of seconds the player has LEFT to complete the level */
	UPROPERTY(EditAnywhere)
	float TimeRemaining;

	/** Keeps track of the game's current state */
	EPlayState CurrentState;

	/** An instance of a SaveGame object to used to load and save high scores */
	class USBBSaveGame* SaveGame;
};
