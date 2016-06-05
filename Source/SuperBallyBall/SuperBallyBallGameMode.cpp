// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperBallyBall.h"
#include "SuperBallyBallGameMode.h"
#include "BallPawn.h"
#include "LevelContainer.h"
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

	bFellBelowLevel = false;
}

// Called when the game starts
void ASuperBallyBallGameMode::BeginPlay()
{
	Super::BeginPlay();
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

		ABallPawn* BallPawn = Cast<ABallPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
		if (BallPawn)
		{
			ALevelContainer* LevelContainer = BallPawn->GetLevelContainer();

			if (LevelContainer && IsBallOutsideLevelBounds(BallPawn, LevelContainer))
			{
				// Reset player, level, and camera positions
				BallPawn->TeleportTo(FVector(0.0f, 0.0f, LevelContainer->GetActorLocation().Z + 100.0f), FRotator::ZeroRotator);
				BallPawn->GetSphereVisual()->SetPhysicsLinearVelocity(FVector::ZeroVector);
				BallPawn->GetSphereVisual()->SetPhysicsAngularVelocity(FVector::ZeroVector);
				BallPawn->GetController()->SetControlRotation(FRotator::ZeroRotator);
				LevelContainer->SetActorRotation(FRotator::ZeroRotator.Quaternion());

				// If statement protects against issue with time deducting twice
				if (!bFellBelowLevel)
				{
					TimeRemaining = TimeRemaining < 5.0f ? 0.0f : TimeRemaining - 5.0f;
				}

				bFellBelowLevel = true;
			}
			else
			{
				bFellBelowLevel = false;

				for (TActorIterator<AGoal> ActorItr(GetWorld()); ActorItr; ++ActorItr)
				{
					AGoal* Goal = *ActorItr;
					if (Goal && HasBallPassedThroughGoal(BallPawn, Goal) && BallPawn->GetColor() == Goal->GetColor())
					{
						SetCurrentState(EPlayState::EWon);
					}
				}
			}
		}

		if (TimeRemaining <= 0.0f)
		{
			SetCurrentState(EPlayState::ELost);
		}
	}
}

// Checks if the ball has fallen below the level or thrown too far outside it
bool ASuperBallyBallGameMode::IsBallOutsideLevelBounds(ABallPawn* BallPawn, ALevelContainer* LevelContainer)
{
	FVector2D BXY = FVector2D(BallPawn->GetActorLocation().X, BallPawn->GetActorLocation().Y);
	float BZ = BallPawn->GetActorLocation().Z;

	FVector2D LXY = FVector2D(LevelContainer->GetActorLocation().X, LevelContainer->GetActorLocation().Y);
	float LZ = LevelContainer->GetActorLocation().Z;
	float LR = LevelContainer->GetSphereComponent()->GetScaledSphereRadius();

	return BZ < LZ - LR || FVector2D::Distance(BXY, LXY) > LR;
}

// Checks if the ball is inside the Goal's static mesh
bool ASuperBallyBallGameMode::HasBallPassedThroughGoal(ABallPawn* BallPawn, AGoal* Goal)
{
	FVector2D BXY = FVector2D(BallPawn->GetActorLocation().X, BallPawn->GetActorLocation().Y);
	float BZ = BallPawn->GetActorLocation().Z;

	FVector2D GXY = FVector2D(Goal->GetActorLocation().X, Goal->GetActorLocation().Y);
	float GZ = Goal->GetActorLocation().Z;
	
	float E = 2.5f;

	return FVector2D::Distance(BXY, GXY) <= 35.0f && BZ > GZ - E && BZ < GZ + E;
}

// Set a new playing state and handle the consequence
void ASuperBallyBallGameMode::SetCurrentState(EPlayState NewState)
{
	CurrentState = NewState;
	ABallPawn* BallPawn = Cast<ABallPawn>(UGameplayStatics::GetPlayerPawn(this, 0));

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
	case EPlayState::EPlaying:
	default:
		break;
	}
}
