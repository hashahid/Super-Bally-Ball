// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "BallPawn.generated.h"

UCLASS()
class SUPERBALLYBALL_API ABallPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABallPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* SphereVisual;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	class AWorldPawn* WorldPawn;

	UPROPERTY(EditAnywhere)
	class UBallPawnMovementComponent* MovementComponent;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	void ChangeRoll(float AxisValue);
	void ChangePitch(float AxisValue);
	void ChangeYaw(float AxisValue);
};
