// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/PPTA_Trace.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Physics/PPCollision.h"
#include "DrawDebugHelpers.h"
#include "Project_P.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PPCharacterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"

APPTA_Trace::APPTA_Trace()
{
	ShouldProduceTargetDataOnServer = false;
}

void APPTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void APPTA_Trace::ConfirmTargetingAndContinue()
{
	//Super::ConfirmTargetingAndContinue();
	PPNET_LOG(LogGAS, Log, TEXT("Begin"));

	if (SourceActor && IsConfirmTargetingAllowed())
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle APPTA_Trace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character);
	if (!ASC)
	{
		PPGAS_LOG(LogGAS, Log, TEXT("ASC Not Found"));
		return FGameplayAbilityTargetDataHandle();
	}

	const UPPCharacterAttributeSet* AttributeSet = ASC->GetSet<UPPCharacterAttributeSet>();
	if (!AttributeSet)
	{
		PPGAS_LOG(LogGAS, Log, TEXT("AttributeSet Not Found"));
		return FGameplayAbilityTargetDataHandle();
	}

	FHitResult OutHitResult;
	const float AttackRadius = AttributeSet->GetAttackRadius();
	const float AttackRange = AttributeSet->GetAttackRange();

	//Params(SCENE_QUERY_STAT(태그이름), 복잡한 트레이스 할지, 충돌 검출 안할 액터)
	//SCENE_QUERY_STAT(태그이름) 들어간 인자로 태그를 생성
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
		TargetDataHandle.Add(TargetData);
	}

#if ENABLE_DRAW_DEBUG

	if (bShowDebug)
	{
		FVector CapsuleOrigine = Start + (End - Start) * 0.5;
		float CapsuleHalfHeight = AttackRange * 0.5;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

		DrawDebugCapsule(GetWorld(), CapsuleOrigine, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(FowardVector).ToQuat(), DrawColor, false, 3.0f);
	}

#endif

	return TargetDataHandle;
}
