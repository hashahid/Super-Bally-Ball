// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperBallyBall.h"
#include "SBBSaveGame.h"

// Sets default values
USBBSaveGame::USBBSaveGame()
{
	Level1PlayerName = TEXT("");
	Level1FastestTime = 30.0f;

	Level2PlayerName = TEXT("");
	Level2FastestTime = 60.0f;

	Level3PlayerName = TEXT("");
	Level3FastestTime = 90.0f;

	SaveSlotName = TEXT("DefaultSaveSlot");
	UserIndex = 0;
}
