// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PPAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Tag/PPGameplayTag.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "Interface/PPAnimInterface.h"

UPPAnimInstance::UPPAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;

	SprintTagContainer.AddTag(PPTAG_CHARACTER_ISSPRINT);
}

void UPPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//AnimInstance가 생성될때 한번만 호출되어 포인터 변수들을 초기화할때 사용
	//#include "Character/PPGASCharacterPlayer.h" 추가
	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		//#include "GameFramework/CharacterMovementComponent.h" 추가
		Movement = Owner->GetCharacterMovement();

		IPPAnimInterface* OwnerInter = Cast<IPPAnimInterface>(Owner);
		if (OwnerInter)
		{
			OwnerInter->BindInputReleasedDelegate(this);
		}
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

	if (Owner)
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
	}
}

void UPPAnimInstance::SaveLastDirection()
{
	LastDirection = CalculateDirection(Velocity , Movement->GetLastUpdateRotation());
}
