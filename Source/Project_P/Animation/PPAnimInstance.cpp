// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PPAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPPAnimInstance::UPPAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
	SprintThreshould = 505.0f;
}

void UPPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//AnimInstance�� �����ɶ� �ѹ��� ȣ��Ǿ� ������ �������� �ʱ�ȭ�Ҷ� ���
	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UPPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		Direction = CalculateDirection(Velocity, Owner->GetActorRotation());
		//Z��(�ϴ� ����)�� ������ X, Y���� ũ�⸦ ��ȯ��
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
		bIsSprint = !bIsIdle & (GroundSpeed > SprintThreshould);
	}
}
