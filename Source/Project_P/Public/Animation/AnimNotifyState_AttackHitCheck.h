// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Interface/PPCombatInterface.h"
#include "AnimNotifyState_AttackHitCheck.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EAttackCollisionType : uint8
{
	LongSword UMETA(DisplayName = "Long Sword Attack (Capsule Sweep)"),
	LeftSword UMETA(DisplayName = "Left Sword Attack (Capsule Sweep)"),
	RightSword UMETA(DisplayName = "Long Sword Attack (Capsule Sweep)"),
	BothSword  UMETA(DisplayName = "Both Sword Attack (Capsule Sweep)")
};

UCLASS()
class PROJECT_P_API UAnimNotifyState_AttackHitCheck : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_AttackHitCheck();

	FString GetNotifyName() const;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	EAttackCollisionType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	int16 Steps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	uint8 bUseDrawDebug : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float WeaponRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Sword")
	FName BaseSocketName = FName("SwordBase");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Sword")
	FName TipSocketName = FName("SwordTip");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Sword")
	FName LeftBaseSocketName = FName("LeftSwordBase");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Sword")
	FName LeftTipSocketName = FName("LeftSwordTip");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Sword")
	FName RightBaseSocketName = FName("RightSwordBase");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Sword")
	FName RightTipSocketName = FName("RightSwordTip");

private:
	UPROPERTY(VisibleAnywere, Category = "Melee")
	TMap<EAttackCollisionType, FVector> PrevBase;

	UPROPERTY(VisibleAnywere, Category = "Melee")
	TMap<EAttackCollisionType, FVector> PrevTip;
};
