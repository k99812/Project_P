// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PPAnimInstance.h"
#include "Character/PPGASCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimMontage.h"

UPPAnimInstance::UPPAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> LevelStartMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project_P/Animation/Montage/Start_Montage.Start_Montage'"));
	if (LevelStartMontageRef.Object)
	{
		LevelStartMontage = LevelStartMontageRef.Object;
	}

	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
	SprintThreshould = 505.0f;
}

void UPPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//AnimInstance가 생성될때 한번만 호출되어 포인터 변수들을 초기화할때 사용
	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}

	APPGASCharacterPlayer* GASPlayer = Cast<APPGASCharacterPlayer>(Owner);
	if (GASPlayer)
	{
		GASPlayer->InputReleasedDelegate.BindUObject(this, &UPPAnimInstance::SaveLastDirection);
	}
}

void UPPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		Direction = CalculateDirection(Velocity, Owner->GetActorRotation());
		//Z축(하늘 방향)을 제외한 X, Y축의 크기를 반환함
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
		bIsSprint = !bIsIdle & (GroundSpeed > SprintThreshould);

		FRotator AimRotation = Owner->GetBaseAimRotation();
		FRotator ActorRotation = Owner->GetActorRotation();
		FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation);
		Roll = DeltaRotation.Roll;
		Yaw = DeltaRotation.Yaw;
		Pitch = DeltaRotation.Pitch;
		//UE_LOG(LogTemp, Log, TEXT("Direction : %f, LastDirection : %f"), Direction, LastDirection);
	}
}

void UPPAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	if (!Montage_IsPlaying(LevelStartMontage))
	{
		Montage_Play(LevelStartMontage, 1.0f);
	}
}

void UPPAnimInstance::SaveLastDirection()
{
	UE_LOG(LogTemp, Log, TEXT("Delegate Callback Funtion Excute"));

	LastDirection = CalculateDirection(Velocity , Movement->GetLastUpdateRotation());
}
