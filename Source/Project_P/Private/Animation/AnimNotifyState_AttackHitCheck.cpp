// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_AttackHitCheck.h"
#include "Interface/PPCombatInterface.h"

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

	AActor* Owner = MeshComp->GetOwner();
	IPPCombatInterface* PPCharacter = Owner ? Cast<IPPCombatInterface>(Owner) : nullptr;
	if (PPCharacter)
	{
		PPCharacter->SetIsSweeping(true);
		PPCharacter->ClearHitActors();

		switch (AttackType)
		{
		case EAttackCollisionType::LeftSword:
			PrevBase[EAttackCollisionType::LeftSword] = MeshComp->GetSocketLocation(LeftBaseSocketName);
			PrevTip[EAttackCollisionType::LeftSword] = MeshComp->GetSocketLocation(LeftTipSocketName);
			break;

		case EAttackCollisionType::RightSword:
			PrevBase[EAttackCollisionType::RightSword] = MeshComp->GetSocketLocation(RightBaseSocketName);
			PrevTip[EAttackCollisionType::RightSword] = MeshComp->GetSocketLocation(RightTipSocketName);
			break;

		case EAttackCollisionType::BothSword:
			PrevBase[EAttackCollisionType::LeftSword] = MeshComp->GetSocketLocation(LeftBaseSocketName);
			PrevTip[EAttackCollisionType::LeftSword] = MeshComp->GetSocketLocation(LeftTipSocketName);
			PrevBase[EAttackCollisionType::RightSword] = MeshComp->GetSocketLocation(RightBaseSocketName);
			PrevTip[EAttackCollisionType::RightSword] = MeshComp->GetSocketLocation(RightTipSocketName);
			break;
		}
	}
}

void UAnimNotifyState_AttackHitCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	AActor* Owner = MeshComp->GetOwner();
	IPPCombatInterface* PPCharacter = Owner ? Cast<IPPCombatInterface>(Owner) : nullptr;
	if (PPCharacter)
	{
		switch (AttackType)
		{
		case EAttackCollisionType::LeftSword:
			PPCharacter->PerformMeleeWeaponSweep(PrevBase[EAttackCollisionType::LeftSword], 
				PrevTip[EAttackCollisionType::LeftSword], MeshComp->GetSocketLocation(LeftBaseSocketName),
				MeshComp->GetSocketLocation(LeftTipSocketName), bUseDrawDebug, Steps);

			PrevBase[EAttackCollisionType::LeftSword] = MeshComp->GetSocketLocation(LeftBaseSocketName);
			PrevTip[EAttackCollisionType::LeftSword] = MeshComp->GetSocketLocation(LeftTipSocketName);

			break;

		case EAttackCollisionType::RightSword:
			PPCharacter->PerformMeleeWeaponSweep(PrevBase[EAttackCollisionType::RightSword],
				PrevTip[EAttackCollisionType::RightSword], MeshComp->GetSocketLocation(RightBaseSocketName),
				MeshComp->GetSocketLocation(RightTipSocketName), bUseDrawDebug, Steps);

			PrevBase[EAttackCollisionType::RightSword] = MeshComp->GetSocketLocation(RightBaseSocketName);
			PrevTip[EAttackCollisionType::RightSword] = MeshComp->GetSocketLocation(RightTipSocketName);

			break;

		case EAttackCollisionType::BothSword:
			PPCharacter->PerformMeleeWeaponSweep(PrevBase[EAttackCollisionType::LeftSword],
				PrevTip[EAttackCollisionType::LeftSword], MeshComp->GetSocketLocation(LeftBaseSocketName),
				MeshComp->GetSocketLocation(LeftTipSocketName), bUseDrawDebug, Steps);
			PPCharacter->PerformMeleeWeaponSweep(PrevBase[EAttackCollisionType::RightSword],
				PrevTip[EAttackCollisionType::RightSword], MeshComp->GetSocketLocation(RightBaseSocketName),
				MeshComp->GetSocketLocation(RightTipSocketName), bUseDrawDebug, Steps);

			PrevBase[EAttackCollisionType::LeftSword] = MeshComp->GetSocketLocation(LeftBaseSocketName);
			PrevTip[EAttackCollisionType::LeftSword] = MeshComp->GetSocketLocation(LeftTipSocketName);
			PrevBase[EAttackCollisionType::RightSword] = MeshComp->GetSocketLocation(RightBaseSocketName);
			PrevTip[EAttackCollisionType::RightSword] = MeshComp->GetSocketLocation(RightTipSocketName);

			break;
		}
	}
}

void UAnimNotifyState_AttackHitCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation);

	AActor* Owner = MeshComp->GetOwner();
	IPPCombatInterface* PPCharacter = Owner ? Cast<IPPCombatInterface>(Owner) : nullptr;
	if (PPCharacter)
	{
		PPCharacter->SetIsSweeping(false);
		PrevBase.Empty();
		PrevTip.Empty();
	}
}
