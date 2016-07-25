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

	// The time it takes to complete a level should be set from the level blueprint
	SetTimeForLevel(0.0f);
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
	FVector2D BYZ = FVector2D(BallPawn->GetActorLocation().Y, BallPawn->GetActorLocation().Z);
	float BX = BallPawn->GetActorLocation().X;
	float BY = BallPawn->GetActorLocation().Y;

	FVector2D GXZ = FVector2D(Goal->GetActorLocation().X, Goal->GetActorLocation().Z);
	FVector2D GYZ = FVector2D(Goal->GetActorLocation().Y, Goal->GetActorLocation().Z);
	float GX = Goal->GetActorLocation().X;
	float GY = Goal->GetActorLocation().Y;
	float GR = 55.0f * Goal->GetActorScale().X;

	float E = 5.0f;

	return (FVector2D::Distance(BXZ, GXZ) <= GR && FMath::Abs(BY - GY) <= E)
		|| (FVector2D::Distance(BYZ, GYZ) <= GR && FMath::Abs(BX - GX) <= E);
}

// Set the time in seconds needed to complete a level
void ASuperBallyBallGameMode::SetTimeForLevel(float Time)
{
	TotalTime = Time;
	TimeRemaining = TotalTime;
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
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		break;
	case EPlayState::EWon:
		// TODO Save TimeRemaining for high score HUD
		if (BallPawn)
		{
			BallPawn->DisableInput(nullptr);
		}
		SetGamePaused(true);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
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
