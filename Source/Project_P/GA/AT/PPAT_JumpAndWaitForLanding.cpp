// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/PPAT_JumpAndWaitForLanding.h"
#include "GameFramework/Character.h"

UPPAT_JumpAndWaitForLanding::UPPAT_JumpAndWaitForLanding()
{
}

UPPAT_JumpAndWaitForLanding* UPPAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwingAbility)
{
	UPPAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<UPPAT_JumpAndWaitForLanding>(OwingAbility);

	return NewTask;
}

void UPPAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();
		
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.AddDynamic(this, &UPPAT_JumpAndWaitForLanding::OnLandedCallBack);
	Character->Jump();

	//�ƹ�Ÿ ���¸� AT�� ���������� Wating ���·� ��ȯ
	SetWaitingOnAvatar();
}

void UPPAT_JumpAndWaitForLanding::OnDestroy(bool bInOwnerFinished)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.RemoveDynamic(this, &UPPAT_JumpAndWaitForLanding::OnLandedCallBack);

	Super::OnDestroy(bInOwnerFinished);
}

void UPPAT_JumpAndWaitForLanding::OnLandedCallBack(const FHitResult& Hit)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}
}
