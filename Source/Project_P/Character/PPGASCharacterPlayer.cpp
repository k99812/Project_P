// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPGASCharacterPlayer.h"
#include "Project_P.h"
#include "AbilitySystemComponent.h"
#include "Player/PPGASPlayerState.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

APPGASCharacterPlayer::APPGASCharacterPlayer()
{
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

	//ī�޶� ��(USpringArmComponent) ����
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	//ĳ���Ϳ� ī�޶��� �Ÿ�
	CameraArm->TargetArmLength = 400.0f;
	//�����̼��� ��Ʈ�ѷ��� �����̼ǰ� ����ȭ ����
	CameraArm->bUsePawnControlRotation = true;

	//ī�޶� ����()
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//ī�޶� ī�޶�Ͽ� �ڽ����� ����
	FollowCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	//�����̼��� ��Ʈ�ѷ��� �����̼ǰ� ����ȭ ����
	FollowCamera->bUsePawnControlRotation = false;
}

UAbilitySystemComponent* APPGASCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void APPGASCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	APPGASPlayerState* GASPlayerState = GetPlayerState<APPGASPlayerState>();
	if (GASPlayerState)
	{
		ASC = GASPlayerState->GetAbilitySystemComponent();
		if (ASC)
		{
			PPGAS_LOG(LogPPGAS, Log, TEXT("ASC is successed"));
			
			ASC->InitAbilityActorInfo(GASPlayerState, this);
		}
	}
}
