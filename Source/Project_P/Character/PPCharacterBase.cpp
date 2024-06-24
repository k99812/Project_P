// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/MovementComponent.h"

// Sets default values
APPCharacterBase::APPCharacterBase()
{
	//폰 설정
	//컨트롤러의 회전(Pitch, Roll, Yaw) 값을 적용할지
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//캡슐 콜라이더 설정
	//추후 사이즈 조정해야됨
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
}


