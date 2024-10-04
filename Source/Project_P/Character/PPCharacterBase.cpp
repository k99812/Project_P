// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/PPComboActionData.h"
#include "Physics/PPCollision.h"

// Sets default values
APPCharacterBase::APPCharacterBase()
{
//�� ����
	//��Ʈ�ѷ��� ȸ��(Pitch, Roll, Yaw) ���� ĳ���Ϳ� ��������
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

//ĸ�� �ݶ��̴� ����
	//#include "Components/CapsuleComponent.h"�߰�
	GetCapsuleComponent()->InitCapsuleSize(25.0f, 92.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PPCAPSULE);

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
	//�ִϸ��̼� ��� ����
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//���̷�Ż �޽� �ݸ��� ����
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
}
