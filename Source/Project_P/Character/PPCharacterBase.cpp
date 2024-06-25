// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APPCharacterBase::APPCharacterBase()
{
	//�� ����
	//��Ʈ�ѷ��� ȸ��(Pitch, Roll, Yaw) ���� ĳ���Ϳ� ��������
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//#include "Components/CapsuleComponent.h"�߰�
	//ĸ�� �ݶ��̴� ����
	//���� ������ �����ؾߵ�
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	//�����Ʈ ����
	//#include "GameFramework/CharacterMovementComponent.h" �߰�
	//�����̴� �������� ĳ���͸� ȸ����ų��
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//ȸ�� �ӵ�
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	//���� �ӵ�
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	//���� ���� ������(���ڰ� Ŭ���� ���߿��� ������ �����ο�(ex) ������ȯ))
	GetCharacterMovement()->AirControl = 0.35f;
	//�ִ� �̵� �ӵ�
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	//���̽�ƽ�� ����Ҷ� ĳ���Ͱ� �����̴� �����ϴ� �ӵ�
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	//���� ���� ����(������)
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	//�޽� ����
	//ĳ���� �𵨸� �ʱ� ��ġ, ȸ���� ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	//�ִϸ��̼� ��� ����
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//���̷�Ż �޽� �ݸ��� ����
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	//���̷��ڸ޽� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonCountess/Characters/Heroes/Countess/Meshes/SM_Countess.SM_Countess'"));
	if (CharacterMeshRef.Object)
	{
		//(SkeletalMesh = ����)�� �ƴ� SetSkeletalMesh(����)�� �ؾ���
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	//�ִϸ��̼��ν��Ͻ�Ŭ���� ����
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT(""));
	if (AnimInstanceClassRef.Class)
	{
		//Set�Լ� ���
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
}


