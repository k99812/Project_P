// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/MovementComponent.h"

// Sets default values
APPCharacterBase::APPCharacterBase()
{
	//�� ����
	//��Ʈ�ѷ��� ȸ��(Pitch, Roll, Yaw) ���� ��������
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//ĸ�� �ݶ��̴� ����
	//���� ������ �����ؾߵ�
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
}


