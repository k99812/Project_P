// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PPPlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPPPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_P_API IPPPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void InitHUD(class UAbilitySystemComponent* ASC) = 0;

	virtual void OnPlayerDead() = 0;

	virtual void RequestRespawn() = 0;
};
