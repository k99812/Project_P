// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PPGameInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPPGameInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_P_API IPPGameInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnPlayerDead() = 0;

	virtual void OnTakeDamage(const float& Damage, const FVector& ActorPosition) = 0;
};
