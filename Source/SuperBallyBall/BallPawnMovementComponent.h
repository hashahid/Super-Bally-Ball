// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "BallPawnMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SUPERBALLYBALL_API UBallPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	UBallPawnMovementComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	// Maximum speed of the pawn in Unreal Units per second
	UPROPERTY(EditAnywhere, Category = MovementSpeed)
	float MaximumSpeed;
};
