// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PPCombatInterface.generated.h"

UENUM(BlueprintType)
enum class EAttackCollisionType : uint8
{
	LongSword UMETA(DisplayName = "Long Sword Attack (Capsule Sweep)"),
	LeftSword UMETA(DisplayName = "Left Sword Attack (Capsule Sweep)"),
	RightSword UMETA(DisplayName = "Right Sword Attack (Capsule Sweep)"),
	BothSword  UMETA(DisplayName = "Both Sword Attack (Capsule Sweep)")
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPPCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_P_API IPPCombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BeginWeaponSweep(EAttackCollisionType AttackType, const FVector& InitBase, const FVector& InitTip) = 0;
	virtual void PerformMeleeWeaponSweep(EAttackCollisionType AttackType, const FVector& CurrBase, const FVector CurrTip, float WeaponRadius, int32 Steps) = 0;
	virtual void EndWeaponSweep() = 0;

	virtual void SetUseDrawDebug(bool InUseDrawDebug) = 0;
	virtual void SetIsSweeping(bool InbIsSweeping) = 0;
	virtual bool GetIsSweeping() const = 0;
};
