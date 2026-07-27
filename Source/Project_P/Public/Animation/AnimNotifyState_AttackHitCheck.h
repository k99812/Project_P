// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Interface/PPCombatInterface.h"
#include "AnimNotifyState_AttackHitCheck.generated.h"

/**
 * 
 */

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

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	EAttackCollisionType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	int Steps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	uint8 bUseDrawDebug : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	float WeaponRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Sword", meta = (AllowPrivateAccess = "true"))
	FName BaseSocketName = FName("SwordBase");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Sword", meta = (AllowPrivateAccess = "true"))
	FName TipSocketName = FName("SwordTip");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Sword", meta = (AllowPrivateAccess = "true"))
	FName LeftBaseSocketName = FName("LeftSwordBase");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Sword", meta = (AllowPrivateAccess = "true"))
	FName LeftTipSocketName = FName("LeftSwordTip");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Sword", meta = (AllowPrivateAccess = "true"))
	FName RightBaseSocketName = FName("RightSwordBase");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Sword", meta = (AllowPrivateAccess = "true"))
	FName RightTipSocketName = FName("RightSwordTip");
};
