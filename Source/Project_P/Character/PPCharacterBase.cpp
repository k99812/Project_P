// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/PPComboActionData.h"
#include "Physics/PPCollision.h"

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

void APPCharacterBase::Multicast_SendPlayMontage(FName Section)
{
	Multicast_PlayMontage(Section);
}

void APPCharacterBase::Multicast_PlayMontage_Implementation(FName Section)
{
	if (!HasAuthority() && GetLocalRole() == ENetRole::ROLE_SimulatedProxy)
	{
		PlayAnimMontage(ComboAttackMontage, 1.0f, Section);
	}
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
