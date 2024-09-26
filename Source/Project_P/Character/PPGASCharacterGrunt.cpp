// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPGASCharacterGrunt.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Attribute/PPCharacterAttributeSet.h"
#include "Data/PPGruntAttributeData.h"

APPGASCharacterGrunt::APPGASCharacterGrunt()
{
//��Ķ��Ż �޽� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Minion_Lane_Melee_Dusk.Minion_Lane_Melee_Dusk'"));
	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
	}

//�ִ��ν��Ͻ� Ŭ���� ����
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Project_P/Blueprint/Grunt/BPAB_Grunt.BPAB_Grunt_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

//��Ʈ����Ʈ ������ �ּ� 
	static ConstructorHelpers::FObjectFinder<UPPGruntAttributeData> AttributeDataRef(TEXT("/Script/Project_P.PPGruntAttributeData'/Game/Project_P/Data/GruntAttributeData.GruntAttributeData'"));
	if (AttributeDataRef.Object)
	{
		AttributeData = AttributeDataRef.Object;
	}

//ĸ�� ������Ʈ ����
	GetCapsuleComponent()->InitCapsuleSize(30.0f, 86.5f);

//�޽� ����
//ĳ���� �޽��� �ʱ� ��ġ, �����̼ǰ� ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -86.5f), FRotator(0.0f, -90.0f, 0.0f));

//ĳ���� �����Ʈ ����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

void APPGASCharacterGrunt::PostInitializeComponents()
{
	Super::PostInitializeComponents();

//���� �ʱ�ȭ
	AttributeSet->InitAttackRadius(AttributeData->AttackRadius);
	AttributeSet->InitMaxAttackRadius(AttributeData->MaxAttackRadius);

	AttributeSet->InitAttackRange(AttributeData->AttackRange);
	AttributeSet->InitMaxAttackRange(AttributeData->MaxAttackRange);

	AttributeSet->InitAttackRate(AttributeData->AttackRate);
	AttributeSet->InitMaxAttackRate(AttributeData->MaxAttackRate);

	AttributeSet->InitMaxHealth(AttributeData->MaxHealth);
	AttributeSet->InitHealth(AttributeSet->GetMaxHealth());
}
