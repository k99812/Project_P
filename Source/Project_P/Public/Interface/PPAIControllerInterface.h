// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PPAIControllerInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFindTargetDelegate, bool, bFindTarget, AActor*, TargetActor);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPPAIControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_P_API IPPAIControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void RunAI() = 0;
	virtual void StopAI() = 0;
	virtual FFindTargetDelegate& GetFindTargetDelegate() = 0;
};
