// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperBallyBall.h"
#include "SuperBallyBallGameMode.h"
#include "BallPawn.h"
#include "LevelContainer.h"
#include "CenterMarker.h"
#include "Goal.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ASuperBallyBallGameMode::ASuperBallyBallGameMode()
{
	// Set the default pawn class to be the BallPawn
	DefaultPawnClass = ABallPawn::StaticClass();

	// Set the default number of seconds to complete the level 
	TimeRemaining = 60.0f;
}

// Called when the game starts
void ASuperBallyBallGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Get a reference to the current level's BallPawn and LevelContainer
	BallPawn = Cast<ABallPawn>(UGameplayStatics::GetPlayerPawn(this, 0));

	// Get a reference to the current level's center
	for (TActorIterator<ACenterMarker> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		LevelCenter = *ActorItr;
		if (LevelCenter)
		{
			break;
		}
	}

	// Get a reference to the current level's goal
	for (TActorIterator<AGoal> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		Goal = *ActorItr;
		if (Goal)
		{
			break;
		}
	}

	SetCurrentState(EPlayState::EPlaying);

	if (HUDWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

// Called every frame
void ASuperBallyBallGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentState == EPlayState::EPlaying)
	{
		TimeRemaining -= DeltaTime;

		if (BallPawn && LevelCenter)
		{
			ALevelContainer* LevelContainer = BallPawn->GetLevelContainer();

			if (IsBallOutsideLevelBounds(BallPawn, LevelCenter))
			{
				BallPawn->ResetPawnAndLevel();
			}
			else if (Goal && HasBallPassedThroughGoal(BallPawn, Goal) && BallPawn->GetColor() == Goal->GetColor())
			{
				SetCurrentState(EPlayState::EWon);
				Goal->PlayAudio();
			}
		}

		if (TimeRemaining <= 0.0f)
		{
			SetCurrentState(EPlayState::ELost);
		}
	}
}

// Checks if the ball has fallen below the level or thrown too far outside it
bool ASuperBallyBallGameMode::IsBallOutsideLevelBounds(ABallPawn* BallPawn, ACenterMarker* LevelCenter)
{
	FVector2D BXY = FVector2D(BallPawn->GetActorLocation().X, BallPawn->GetActorLocation().Y);
	float BZ = BallPawn->GetActorLocation().Z;

	FVector2D CXY = FVector2D(LevelCenter->GetActorLocation().X, LevelCenter->GetActorLocation().Y);
	float CZ = LevelCenter->GetActorLocation().Z;

	float R = BallPawn->GetLevelContainer()->GetSphereComponent()->GetScaledSphereRadius();

	return BZ < CZ - R || FVector2D::Distance(BXY, CXY) > R;
}

// Checks if the ball is inside the Goal's static mesh
bool ASuperBallyBallGameMode::HasBallPassedThroughGoal(ABallPawn* BallPawn, AGoal* Goal)
{
	FVector2D BXZ = FVector2D(BallPawn->GetActorLocation().X, BallPawn->GetActorLocation().Z);
	float BY = BallPawn->GetActorLocation().Y;

	FVector2D GXZ = FVector2D(Goal->GetActorLocation().X, Goal->GetActorLocation().Z);
	float GY = Goal->GetActorLocation().Y;

	float E = 5.0f;

	// Goal unscaled inner radius = 75.0f, scaled 2x = 150.0f
	// 150.0f - 40.0f (BallPawn's radius) = 110.0f
	return FVector2D::Distance(BXZ, GXZ) <= 110.0f && BY > GY - E && BY < GY + E;
}

// Set a new playing state and handle the consequence
void ASuperBallyBallGameMode::SetCurrentState(EPlayState NewState)
{
	CurrentState = NewState;

	switch (NewState)
	{
	case EPlayState::ELost:
		TimeRemaining = 0.0f;
		if (BallPawn)
		{
			BallPawn->DisableInput(nullptr);
		}
		break;
	case EPlayState::EWon:
		// TODO Save TimeRemaining for high score HUD
		if (BallPawn)
		{
			BallPawn->DisableInput(nullptr);
		}
		SetGamePaused(true);
	case EPlayState::EPlaying:
	default:
		break;
	}
}

// Pause or unpause the game
void ASuperBallyBallGameMode::SetGamePaused(bool bIsPaused)
{
	APlayerController* const MyPlayer = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	if (MyPlayer != NULL)
	{
		MyPlayer->SetPause(bIsPaused);
	}
}
