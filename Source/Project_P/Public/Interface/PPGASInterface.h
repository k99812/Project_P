// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PPGASInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPPGASInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_P_API IPPGASInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual TObjectPtr<UAnimMontage> GetComboAttackMontage() = 0;

	virtual TObjectPtr<class UPPComboActionData> GetComboActionData() = 0;

	virtual void Multicast_SendPlayMontage(const FName& Section) = 0;
};
