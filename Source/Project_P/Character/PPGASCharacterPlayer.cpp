// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPGASCharacterPlayer.h"
#include "Project_P.h"
#include "AbilitySystemComponent.h"
#include "Player/PPGASPlayerState.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

APPGASCharacterPlayer::APPGASCharacterPlayer()
{
	//스켈레텔메쉬 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonCountess/Characters/Heroes/Countess/Meshes/SM_Countess.SM_Countess'"));
	if (CharacterMeshRef.Object)
	{
		//(SkeletalMesh = 변수)가 아닌 SetSkeletalMesh(변수)로 해야함
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	//애니메이션인스턴스클래스 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT(""));
	if (AnimInstanceClassRef.Class)
	{
		//Set함수 사용
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	//카메라 암(USpringArmComponent) 설정
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	//캐릭터와 카메라의 거리
	CameraArm->TargetArmLength = 270.0f;
	//로테이션을 컨트롤러의 로테이션과 동기화 할지
	CameraArm->bUsePawnControlRotation = true;

	//카메라 설정()
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//카메라를 카메라암에 자식으로 붙임
	FollowCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	//로테이션을 컨트롤러의 로테이션과 동기화 할지
	FollowCamera->bUsePawnControlRotation = false;

	//인풋 설정
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT(""));
	if (InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveInputActionRef(TEXT(""));
	if (MoveInputActionRef.Object)
	{
		MoveAction = MoveInputActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpInputActionRef(TEXT(""));
	if (JumpInputActionRef.Object)
	{
		JumpAction = JumpInputActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookInputActionRef(TEXT(""));
	if (LookInputActionRef.Object)
	{
		LookAction = LookInputActionRef.Object;
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

void APPGASCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void APPGASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//#include "EnhancedInputComponent.h" 추가
	//#include "EnhancedInputSubsystems.h" 추가

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APPGASCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APPGASCharacterPlayer::Look);
}

void APPGASCharacterPlayer::Move(const FInputActionValue& Value)
{
}

void APPGASCharacterPlayer::Look(const FInputActionValue& Value)
{
}
