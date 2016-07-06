// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class SUPERBALLYBALL_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	// Called after construction and before BeginPlay
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/** Return the color of the pickup's mesh's material */
	FORCEINLINE FLinearColor GetColor() const { return Color; }

	/** Return whether or not the pickup is active */
	FORCEINLINE bool IsActive() const { return bIsActive; }

	/** Allow other classes to change activeness of pickup */
	void SetActive(bool NewPickupState);

private:
	/** Static mesh to represent the pickup */
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* PickupVisual;

	/** The color of the pickup's mesh's material. It transfers to the BallPawn on overlap */
	UPROPERTY(EditAnywhere)
	FLinearColor Color;

	/** True when the pickup can be collected and false when it is deactivated */
	bool bIsActive;
};
