// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPGASCharacterGrunt.h"
#include "Components/CapsuleComponent.h"

APPGASCharacterGrunt::APPGASCharacterGrunt()
{
//스캘레탈 메쉬 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Minion_Lane_Melee_Dusk.Minion_Lane_Melee_Dusk'"));
	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
	}

//애니메이션인스턴스클래스 설정
	/*static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT(""));
	if (AnimInstanceClassRef.Class)
	{
		//Set함수 사용
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}*/

//캡슐 컴포넌트 설정
	GetCapsuleComponent()->InitCapsuleSize(25.0f, 90.0f);

//메쉬 설정
//캐릭터 메쉬의 초기 위치, 로테이션값 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -95.0f), FRotator(0.0f, -90.0f, 0.0f));
}
