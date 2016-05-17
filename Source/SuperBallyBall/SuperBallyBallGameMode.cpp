// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperBallyBall.h"
#include "SuperBallyBallGameMode.h"
#include "BallPawn.h"

ASuperBallyBallGameMode::ASuperBallyBallGameMode()
{
	DefaultPawnClass = ABallPawn::StaticClass();
}
