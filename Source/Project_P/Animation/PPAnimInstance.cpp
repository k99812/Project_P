// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PPAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Tag/PPGameplayTag.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "Interface/PPAnimInterface.h"

UPPAnimInstance::UPPAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;

	SprintTagContainer.AddTag(PPTAG_CHARACTER_ISSPRINT);
}

void UPPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//AnimInstance�� �����ɶ� �ѹ��� ȣ��Ǿ� ������ �������� �ʱ�ȭ�Ҷ� ���
	//#include "Character/PPGASCharacterPlayer.h" �߰�
	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		//#include "GameFramework/CharacterMovementComponent.h" �߰�
		Movement = Owner->GetCharacterMovement();

		IPPAnimInterface* OwnerInter = Cast<IPPAnimInterface>(Owner);
		if (OwnerInter)
		{
			OwnerInter->BindInputReleasedDelegate(this);
		}
	}
}

void UPPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		//CalculateDirection(FVector, FRotator)
		Direction = CalculateDirection(Velocity, Owner->GetActorRotation());
		//Z��(�ϴ� ����)�� ������ X, Y���� ũ�⸦ ��ȯ��
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);

		FRotator AimRotation = Owner->GetBaseAimRotation();
		FRotator ActorRotation = Owner->GetActorRotation();
		//#include "Kismet/KismetMathLibrary.h" �߰�
		FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation);
		Roll = DeltaRotation.Roll;
		Yaw = DeltaRotation.Yaw;
		Pitch = DeltaRotation.Pitch;
	}
	if (ASC)
	{
		bIsSprint = ASC->HasAnyMatchingGameplayTags(SprintTagContainer);
	}
}

void UPPAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	if (Owner)
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
	}
}

void UPPAnimInstance::SaveLastDirection()
{
	LastDirection = CalculateDirection(Velocity , Movement->GetLastUpdateRotation());
}
