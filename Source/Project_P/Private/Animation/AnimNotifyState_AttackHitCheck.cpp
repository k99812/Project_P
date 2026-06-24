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
		PPCharacter->bIsSweeping = true;
		PPCharacter->HitActors.Empty();
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
				PPCharacter->PerformMeleeWeaponSweep(MeshComp->GetSocketLocation(LeftBaseSocketName),
					MeshComp->GetSocketLocation(LeftTipSocketName), Steps);
				break;

			case EAttackCollisionType::RightSword:
				PPCharacter->PerformMeleeWeaponSweep(MeshComp->GetSocketLocation(RightBaseSocketName),
					MeshComp->GetSocketLocation(RightTipSocketName), Steps);
				break;

			case EAttackCollisionType::BothSword:
				PPCharacter->PerformMeleeWeaponSweep(MeshComp->GetSocketLocation(LeftBaseSocketName),
					MeshComp->GetSocketLocation(LeftTipSocketName), Steps);
				PPCharacter->PerformMeleeWeaponSweep(MeshComp->GetSocketLocation(RightBaseSocketName),
					MeshComp->GetSocketLocation(RightTipSocketName), Steps);
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
		PPCharacter->bIsSweeping = false;
	}
}
