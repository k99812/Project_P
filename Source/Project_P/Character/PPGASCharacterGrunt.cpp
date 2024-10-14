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
//스캘레탈 메쉬 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Minion_Lane_Melee_Dusk.Minion_Lane_Melee_Dusk'"));
	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
	}

//애님인스턴스 클래스 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Project_P/Blueprint/Grunt/BPAB_Grunt.BPAB_Grunt_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

//어트리뷰트 데이터 애셋 
	static ConstructorHelpers::FObjectFinder<UPPGruntAttributeData> AttributeDataRef(TEXT("/Script/Project_P.PPGruntAttributeData'/Game/Project_P/Data/GruntAttributeData.GruntAttributeData'"));
	if (AttributeDataRef.Object)
	{
		AttributeData = AttributeDataRef.Object;
	}

//콤보 
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

//캡슐 컴포넌트 설정
	GetCapsuleComponent()->InitCapsuleSize(30.0f, 86.5f);

//메쉬 설정
//캐릭터 메쉬의 초기 위치, 로테이션값 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -86.5f), FRotator(0.0f, -90.0f, 0.0f));

//캐랙터 무브먼트 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

//HPBar 설정
	HpBar = CreateDefaultSubobject<UPPGASWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 175.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/Project_P/Blueprint/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		//위젯 컴포넌트에 위젯 설정
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(200.0f, 20.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

//Dead 몽타주 설정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project_P/Animation/Montage/GruntDead_Montage.GruntDead_Montage'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}
}

void APPGASCharacterGrunt::PostInitializeComponents()
{
	Super::PostInitializeComponents();

//스탯 초기화
	AttributeSet->InitAttackRadius(AttributeData->AttackRadius);
	AttributeSet->InitMaxAttackRadius(AttributeData->MaxAttackRadius);

	AttributeSet->InitAttackRange(AttributeData->AttackRange);
	AttributeSet->InitMaxAttackRange(AttributeData->MaxAttackRange);

	AttributeSet->InitAttackRate(AttributeData->AttackRate);
	AttributeSet->InitMaxAttackRate(AttributeData->MaxAttackRate);

	AttributeSet->InitMaxHealth(AttributeData->MaxHealth);
	AttributeSet->InitHealth(AttributeSet->GetMaxHealth());
}