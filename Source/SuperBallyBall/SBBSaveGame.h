// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "SBBSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SUPERBALLYBALL_API USBBSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	/** Name of the player who completed Level 1 the fastest */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "High Score")
	FString Level1PlayerName;

	/** Name of the player who completed Level 2 the fastest */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "High Score")
	FString Level2PlayerName;

	/** Name of the player who completed Level 3 the fastest */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "High Score")
	FString Level3PlayerName;

	/** The lowest amount of time in seconds used to complete Level 1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "High Score")
	float Level1FastestTime;

	/** The lowest amount of time in seconds used to complete Level 2 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "High Score")
	float Level2FastestTime;

	/** The lowest amount of time in seconds used to complete Level 3 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "High Score")
	float Level3FastestTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Basic)
	int32 UserIndex;

	// Sets default values for this save game's properties
	USBBSaveGame();
};
