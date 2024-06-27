// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/PPGASPlayerState.h"
#include "Project_P.h"

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
