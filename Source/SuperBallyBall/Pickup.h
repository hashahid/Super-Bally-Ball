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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Return whether or not the pickup is active */
	FORCEINLINE bool IsActive() const { return bIsActive; }

	/** Allow other classes to change activeness of pickup */
	void SetActive(bool NewPickupState);

	/** Function to call when the pickup is collected */
	virtual void WasCollected();

private:
	/** Static mesh to represent the pickup */
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* PickupVisual;

	/** True when the pickup can be collected and false when it is deactivated */
	bool bIsActive;
};
