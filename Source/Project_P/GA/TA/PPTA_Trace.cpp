// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/PPTA_Trace.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Physics/PPCollision.h"

APPTA_Trace::APPTA_Trace()
{
}

void APPTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void APPTA_Trace::ConfirmTargetingAndContinue()
{
	Super::ConfirmTargetingAndContinue();

	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle APPTA_Trace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	FHitResult OutHitResult;
	const float AttackRadius = 50.0f;
	const float AttackRange = 100.0f;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(PPTA_Trace), false, Character);
	const FVector FowardVector = Character->GetActorForwardVector();
	const FVector Start = Character->GetActorLocation() + FowardVector * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + FowardVector * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_PPACTION, 
		FCollisionShape::MakeSphere(AttackRadius), Params);

	FGameplayAbilityTargetDataHandle TargetDataHandle;
	if (HitDetected)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		TargetDataHandle.Append(TargetData);
	}

#if ENABLE_DRAW_DEBUG

	if (bSHowDebug)
	{
		FVector CapsuleOrigine = Start + (End - Start) * 0.5;
		float CapsuleHalfHeight = AttackRange * 0.5;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

		DrawDebugCapsule(GetWorld(), CapsuleOrigine, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(FowardVector).ToQuat(), DrawColor, false, 5.0f);
	}

#endif

	return TargetDataHandle;
}
