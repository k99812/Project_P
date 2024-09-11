// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PPAnimInstance.h"
#include "Character/PPGASCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimMontage.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Tag/PPGameplayTag.h"

UPPAnimInstance::UPPAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> LevelStartMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project_P/Animation/Montage/Start_Montage.Start_Montage'"));
	if (LevelStartMontageRef.Object)
	{
		LevelStartMontage = LevelStartMontageRef.Object;
	}

	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;

	SprintTagContainer.AddTag(PPTAG_CHARACTER_ISSPRINT);
}

void UPPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//AnimInstance가 생성될때 한번만 호출되어 포인터 변수들을 초기화할때 사용
	//#include "Character/PPGASCharacterPlayer.h" 추가
	Owner = Cast<APPGASCharacterPlayer>(GetOwningActor());
	if (Owner)
	{
		//#include "GameFramework/CharacterMovementComponent.h" 추가
		Movement = Owner->GetCharacterMovement();
		Owner->InputReleasedDelegate.BindUObject(this, &UPPAnimInstance::SaveLastDirection);
	}
}

void UPPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		//CalculateDirection(FVector, FRotator)
		Direction = CalculateDirection(Velocity, Owner->GetActorRotation());
		//Z축(하늘 방향)을 제외한 X, Y축의 크기를 반환함
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);

		FRotator AimRotation = Owner->GetBaseAimRotation();
		FRotator ActorRotation = Owner->GetActorRotation();
		//#include "Kismet/KismetMathLibrary.h" 추가
		FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation);
		Roll = DeltaRotation.Roll;
		Yaw = DeltaRotation.Yaw;
		Pitch = DeltaRotation.Pitch;
	}
	if (ASC)
	{
		bIsSprint = ASC->HasAnyMatchingGameplayTags(SprintTagContainer);
	}
}

void UPPAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	//#include "Animation/AnimMontage.h" 추가
	if (!Montage_IsPlaying(LevelStartMontage))
	{
		Montage_Play(LevelStartMontage, 1.0f);
	}

	if (Owner)
	{
		ASC = Owner->GetAbilitySystemComponent();
	}
}

void UPPAnimInstance::SaveLastDirection()
{
	LastDirection = CalculateDirection(Velocity , Movement->GetLastUpdateRotation());
}
