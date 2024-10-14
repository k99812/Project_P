// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPGASCharacterGrunt.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Attribute/PPCharacterAttributeSet.h"
#include "Data/PPGruntAttributeData.h"
#include "Data/PPComboActionData.h"
#include "UI/PPGASWidgetComponent.h"
#include "UI/PPGASUserWidget.h"

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

//�޺� 
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project_P/Animation/Montage/GruntAttack_Montage.GruntAttack_Montage'"));
	if (ComboAttackMontageRef.Object)
	{
		ComboAttackMontage = ComboAttackMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPPComboActionData> ComboActionDataRef(TEXT("/Script/Project_P.PPComboActionData'/Game/Project_P/Data/GruntComboActionData.GruntComboActionData'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
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

//HPBar ����
	HpBar = CreateDefaultSubobject<UPPGASWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 175.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/Project_P/Blueprint/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		//���� ������Ʈ�� ���� ����
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(200.0f, 20.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

//Dead ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project_P/Animation/Montage/GruntDead_Montage.GruntDead_Montage'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}
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