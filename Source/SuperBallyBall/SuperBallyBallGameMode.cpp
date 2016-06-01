// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperBallyBall.h"
#include "SuperBallyBallGameMode.h"
#include "BallPawn.h"
#include "LevelContainer.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ASuperBallyBallGameMode::ASuperBallyBallGameMode()
{
	// Set the default pawn class to be the BallPawn
	DefaultPawnClass = ABallPawn::StaticClass();

	// Set the default number of seconds to complete the level 
	TimeRemaining = 60.0f;

	bTickCaseExecuted = false;
}

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
			// If the ball falls below the level
			if (LevelContainer && BallPawn->GetActorLocation().Z < LevelContainer->GetActorLocation().Z - LevelContainer->GetSphereComponent()->GetScaledSphereRadius() - 100.0f)
			{
				// UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);

				// Reset player position
				BallPawn->TeleportTo(FVector(0.0f, 0.0f, LevelContainer->GetActorLocation().Z + 100.0f), FRotator(0.0f));
				BallPawn->GetSphereVisual()->SetPhysicsLinearVelocity(FVector(0.0f));
				BallPawn->GetSphereVisual()->SetPhysicsAngularVelocity(FVector(0.0f));
				LevelContainer->SetActorRotation(FRotator(0.0f).Quaternion());

				// Penalize player maximum of 5 seconds for falling off the level
				if (!bTickCaseExecuted)
				{
					TimeRemaining = TimeRemaining < 5.0f ? 0.0f : TimeRemaining - 5.0f;
				}

				bTickCaseExecuted = true;
			}
			else
			{
				bTickCaseExecuted = false;
			}
		}

		if (TimeRemaining <= 0.0f)
		{
			SetCurrentState(EPlayState::ELost);
		}
	}
}

void ASuperBallyBallGameMode::HandleNewState(EPlayState NewState)
{
	ABallPawn* BallPawn = Cast<ABallPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	switch (NewState)
	{
	case EPlayState::ELost:
		if (BallPawn)
		{
			BallPawn->DisableInput(nullptr);
		}
		break;
	case EPlayState::EWon:
	case EPlayState::EPlaying:
	default:
		break;
	}
}

void ASuperBallyBallGameMode::SetCurrentState(EPlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}
