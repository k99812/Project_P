// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_AttackHitCheck.h"
#include "Interface/PPCombatInterface.h"
#include "Project_P.h"

UAnimNotifyState_AttackHitCheck::UAnimNotifyState_AttackHitCheck()
{

}

FString UAnimNotifyState_AttackHitCheck::GetNotifyName() const
{
	return FString(TEXT("MeleeAttackHitCheck"));
}

void UAnimNotifyState_AttackHitCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	PPGAS_LOG(LogGAS, Log, TEXT("MeleeAttack NotifyBegin"));

	APawn* Owner = Cast<APawn>(MeshComp->GetOwner());

	if (!Owner || !Owner->IsLocallyControlled()) return;

	if (IPPCombatInterface* PPCharacter = Cast<IPPCombatInterface>(Owner))
	{
		PPCharacter->SetIsSweeping(true);
		PPCharacter->SetUseDrawDebug(bUseDrawDebug);

		switch (AttackType)
		{
		case EAttackCollisionType::LeftSword:
			PPCharacter->BeginWeaponSweep(EAttackCollisionType::LeftSword, 
				MeshComp->GetSocketLocation(LeftBaseSocketName), 
				MeshComp->GetSocketLocation(LeftTipSocketName));
			break;

		case EAttackCollisionType::RightSword:
			PPCharacter->BeginWeaponSweep(EAttackCollisionType::RightSword,
				MeshComp->GetSocketLocation(RightBaseSocketName),
				MeshComp->GetSocketLocation(RightTipSocketName));
			break;

		case EAttackCollisionType::BothSword:
			PPCharacter->BeginWeaponSweep(EAttackCollisionType::LeftSword,
				MeshComp->GetSocketLocation(LeftBaseSocketName),
				MeshComp->GetSocketLocation(LeftTipSocketName));
			PPCharacter->BeginWeaponSweep(EAttackCollisionType::RightSword,
				MeshComp->GetSocketLocation(RightBaseSocketName),
				MeshComp->GetSocketLocation(RightTipSocketName));
			break;
		}
	}
}

void UAnimNotifyState_AttackHitCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	APawn* Owner = Cast<APawn>(MeshComp->GetOwner());

	if (!Owner || !Owner->IsLocallyControlled()) return;

	if (IPPCombatInterface* PPCharacter = Cast<IPPCombatInterface>(Owner))
	{
		switch (AttackType)
		{
		case EAttackCollisionType::LeftSword:
			PPCharacter->PerformMeleeWeaponSweep(EAttackCollisionType::LeftSword, MeshComp->GetSocketLocation(LeftBaseSocketName), 
				MeshComp->GetSocketLocation(LeftTipSocketName), WeaponRadius, Steps);
			break;

		case EAttackCollisionType::RightSword:
			PPCharacter->PerformMeleeWeaponSweep(EAttackCollisionType::RightSword, MeshComp->GetSocketLocation(RightBaseSocketName),
				MeshComp->GetSocketLocation(RightTipSocketName), WeaponRadius, Steps);
			break;

		case EAttackCollisionType::BothSword:
			PPCharacter->PerformMeleeWeaponSweep(EAttackCollisionType::LeftSword, MeshComp->GetSocketLocation(LeftBaseSocketName),
				MeshComp->GetSocketLocation(LeftTipSocketName), WeaponRadius, Steps);
			PPCharacter->PerformMeleeWeaponSweep(EAttackCollisionType::RightSword, MeshComp->GetSocketLocation(RightBaseSocketName),
				MeshComp->GetSocketLocation(RightTipSocketName), WeaponRadius, Steps);
			break;
		}
	}
}

void UAnimNotifyState_AttackHitCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation);

	PPGAS_LOG(LogGAS, Log, TEXT("MeleeAttack NotifyEnd"));

	APawn* Owner = Cast<APawn>(MeshComp->GetOwner());

	if (!Owner || !Owner->IsLocallyControlled()) return;

	if (IPPCombatInterface* PPCharacter = Cast<IPPCombatInterface>(Owner))
	{
		PPCharacter->SetIsSweeping(false);
		PPCharacter->EndWeaponSweep();
	}
}
