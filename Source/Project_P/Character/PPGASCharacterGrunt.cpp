// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPGASCharacterGrunt.h"
#include "Components/CapsuleComponent.h"

APPGASCharacterGrunt::APPGASCharacterGrunt()
{
//��Ķ��Ż �޽� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Minion_Lane_Melee_Dusk.Minion_Lane_Melee_Dusk'"));
	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
	}

//�ִϸ��̼��ν��Ͻ�Ŭ���� ����
	/*static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT(""));
	if (AnimInstanceClassRef.Class)
	{
		//Set�Լ� ���
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}*/

//ĸ�� ������Ʈ ����
	GetCapsuleComponent()->InitCapsuleSize(25.0f, 90.0f);

//�޽� ����
//ĳ���� �޽��� �ʱ� ��ġ, �����̼ǰ� ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -95.0f), FRotator(0.0f, -90.0f, 0.0f));
}
