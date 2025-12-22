// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PPMonsterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPPMonsterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_P_API IPPMonsterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetMonstHpBarVisibility(bool bVisible) = 0;
};
