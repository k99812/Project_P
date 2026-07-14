// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPCharacterBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/PPComboActionData.h"
#include "Physics/PPCollision.h"
#include "Tag/PPGameplayTag.h"
#include "Project_P.h"

// Sets default values
APPCharacterBase::APPCharacterBase()
{
//폰 설정
	//컨트롤러의 회전(Pitch, Roll, Yaw) 값을 캐릭터에 적용할지
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
//캡슐 콜라이더 설정
	//#include "Components/CapsuleComponent.h"추가
	GetCapsuleComponent()->InitCapsuleSize(25.0f, 92.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PPCAPSULE);

//무브먼트 설정
	//#include "GameFramework/CharacterMovementComponent.h" 추가
	//움직이는 방향으로 캐릭터를 회전시킬지
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//회전 속도
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	//점프 속도
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	//공중 조작 자유도(숫자가 클수록 공중에서 조작이 자유로움(ex) 방향전환))
	GetCharacterMovement()->AirControl = 0.35f;
	//최대 이동 속도
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	//조이스틱을 사용할때 캐릭터가 움직이는 시작하는 속도
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	//제동 감속 보행(마찰력)
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

//메쉬 설정
	//애니메이션 모드 지정
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//스켈레탈 메쉬 콜리전 설정
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
}

void APPCharacterBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = GameplayTags;
}

void APPCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
}

void APPCharacterBase::SetAlive()
{
}

void APPCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	if (DeadMontage)
	{
		AnimInstance->Montage_Play(DeadMontage, 1.0f);
	}
}

void APPCharacterBase::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	GenericTeamId = TeamID;
}

FGenericTeamId APPCharacterBase::GetGenericTeamId() const
{
	return GenericTeamId;
}

void APPCharacterBase::BeginWeaponSweep(EAttackCollisionType AttackType, const FVector& InitBase, const FVector& InitTip)
{
	PrevBaseMap.Add(AttackType, InitBase);
	PrevTipMap.Add(AttackType, InitTip);
}

void APPCharacterBase::EndWeaponSweep()
{
	HitActors.Empty();
	PrevBaseMap.Empty();
	PrevTipMap.Empty();
}

void APPCharacterBase::PerformMeleeWeaponSweep(EAttackCollisionType AttackType, const FVector& CurrBase, const FVector CurrTip, float WeaponRadius, int32 Steps)
{
	if (!PrevBaseMap.Contains(AttackType) || !PrevTipMap.Contains(AttackType)) return;

	FCollisionQueryParams Params(FName(TEXT("Melee")), true, this);

	FVector PrevBase = PrevBaseMap[AttackType], PrevTip = PrevTipMap[AttackType];
	FVector StepPrevBase = PrevBase, StepPrevTip = PrevTip;

	for (int i = 1; i <= Steps; i++)
	{
		float Alpha = (float)i / (float)Steps;

		FVector StepCurrBase = FMath::Lerp(PrevBase, CurrBase, Alpha);
		FVector StepCurrTip = FMath::Lerp(PrevTip, CurrTip, Alpha);

		FVector PrevCenter = (StepPrevBase + StepPrevTip) * 0.5f;
		FVector CurrCenter = (StepCurrBase + StepCurrTip) * 0.5f;
		float WeaponLength = FVector::Distance(StepCurrBase, StepCurrTip);

		FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(WeaponRadius, WeaponLength * 0.5f);
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(StepCurrTip - StepCurrBase).ToQuat();

		TArray<FHitResult> HitResult;

		bool bHit = GetWorld()->SweepMultiByChannel(
			HitResult,
			PrevCenter,
			CurrCenter,
			CapsuleRot,
			CCHANNEL_PPACTION,
			CollisionShape,
			Params
		);

		if (bHit)
		{
			for (const FHitResult& Hit : HitResult)
			{
				AActor* HitActor = Hit.GetActor();

				if (HitActor)
				{
					if (HitActors.Contains(HitActor)) continue;

					HitActors.Emplace(HitActor);

					FGameplayAbilityTargetData_SingleTargetHit* TargetData =
						new FGameplayAbilityTargetData_SingleTargetHit(Hit);

					FGameplayAbilityTargetDataHandle DataHandle;
					DataHandle.Add(TargetData);

					FGameplayEventData Payload;
					Payload.Target = HitActor;
					Payload.TargetData = DataHandle;
					Payload.Instigator = this;

					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, PPTAG_EVENT_MELEEHIT, Payload);

					//히트 이벤트 발동
					PPGAS_LOG(LogGAS, Log, TEXT("Hit Event Activate : %s"), *HitActor->GetName());
				}
			}
		}

#if ENABLE_DRAW_DEBUG
		if (bUseDrawDebug)
		{
			MeleeAttackDebugDraw(HitResult, PrevCenter, CurrCenter, CapsuleRot, WeaponLength, WeaponRadius, bHit);
		}
#endif

		StepPrevBase = StepCurrBase;
		StepPrevTip = StepCurrTip;
	}

	PrevBaseMap[AttackType] = CurrBase;
	PrevTipMap[AttackType] = CurrTip;
}

#if ENABLE_DRAW_DEBUG
void APPCharacterBase::MeleeAttackDebugDraw(const TArray<FHitResult>& HitResult, const FVector& PrevCenter, const FVector& CurrCenter, const FQuat& CapsuleRot, const float WeaponLength, const float WeaponRadius, const bool bHit) const
{
	FColor DrawColor = bHit ? FColor::Green : FColor::Red;
	float DrawLifeTime = 2.0f;

	DrawDebugCapsule(GetWorld(), CurrCenter, WeaponLength * 0.5f, WeaponRadius, CapsuleRot, DrawColor,
		false, DrawLifeTime, 0, 0.5f);

	DrawDebugLine(GetWorld(), PrevCenter, CurrCenter, DrawColor, false, DrawLifeTime, 0, 1.0f);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResult)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 8.0f, 12, FColor::Yellow, false,
				DrawLifeTime, 0, 1.0f);
		}
	}
}
#endif