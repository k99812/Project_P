// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h"
#include "Input/PPInputEnum.h"
#include "GenericTeamAgentInterface.h"
#include "Interface/PPGASInterface.h"
#include "Interface/PPCharacterBaseInterface.h"
#include "Interface/PPCombatInterface.h"
#include "PPCharacterBase.generated.h"

UCLASS()
class PROJECT_P_API APPCharacterBase : public ACharacter, public IGameplayTagAssetInterface, public IGenericTeamAgentInterface, public IPPGASInterface, public IPPCharacterBaseInterface, public IPPCombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APPCharacterBase();

	FORCEINLINE virtual TObjectPtr<UAnimMontage> GetComboAttackMontage() override { return ComboAttackMontage; }
	FORCEINLINE virtual TObjectPtr<class UPPComboActionData> GetComboActionData() override { return ComboActionData; }

	UFUNCTION(BlueprintCallable, Category = "Tags")
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	TObjectPtr<UAnimMontage> ComboAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	TObjectPtr<class UPPComboActionData> ComboActionData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dead", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags", Meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer GameplayTags;

// Dead Section
public:
	virtual void SetDead() override;

	virtual void SetAlive() override;

protected:
	virtual void PlayDeadAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dead", Meta = (AllowPrivateAccess = "true"))
	float DeadEventDelayTime = 5.0f;

//GenericTeam Section
public:
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	FGenericTeamId GenericTeamId;

//Melee Attack Section
public:
	virtual void BeginWeaponSweep(EAttackType AttackType, const FVector& InitBase, const FVector& InitTip) override;
	virtual void EndWeaponSweep() override;

	virtual void PerformMeleeWeaponSweep(EAttackType AttackType, const FVector& CurrBase, const FVector CurrTip, float WeaponRadius, int32 Steps) override;

	virtual void SetUseDrawDebug(bool InUseDrawDebug) override { bUseDrawDebug = InUseDrawDebug; }
	virtual void SetIsSweeping(bool InbIsSweeping) override { bIsSweeping = InbIsSweeping; }
	virtual bool GetIsSweeping() const override { return bIsSweeping; }

protected:
	void MeleeAttackDebugDraw(const TArray<FHitResult>& HitResult, const FVector& PrevCenter, const FVector& CurrCenter, const FQuat& CapsuleRot, const float WeaponLength, const float WeaponRadius, const bool bHit) const;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Melee")
	uint8 bUseDrawDebug : 1 = false;

	UPROPERTY(VisibleAnywhere, Category = "Melee")
	uint8 bIsSweeping : 1 = false;

	UPROPERTY(VisibleAnywhere, Category = "Melee")
	TArray<AActor*> HitActors;

	UPROPERTY()
	TMap<EAttackType, FVector> PrevBaseMap;

	UPROPERTY()
	TMap<EAttackType, FVector> PrevTipMap;
};
