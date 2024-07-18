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
#include "GameFramework/CharacterMovementComponent.h"

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
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Project_P/Blueprint/BP_PPABCountess.BP_PPABCountess_C"));
	if (AnimInstanceClassRef.Class)
	{
		//Set함수 사용
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

//캐랙터 무브먼트 설정
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 2000.0f, 0.0f);

//카메라 암(USpringArmComponent) 설정
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	//캐릭터와 카메라 암의 거리
	CameraArm->TargetArmLength = 300.0f;
	//로테이션을 컨트롤러의 로테이션과 동기화 할지
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));

//카메라 설정(UCameraComponent)
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//카메라를 카메라암에 자식으로 붙임
	FollowCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	//로테이션을 컨트롤러의 로테이션과 동기화 할지
	FollowCamera->bUsePawnControlRotation = false;

//인풋 설정
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Project_P/Input/IMC_Third.IMC_Third'"));
	if (InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveInputActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Project_P/Input/Actions/IA_Move.IA_Move'"));
	if (MoveInputActionRef.Object)
	{
		MoveAction = MoveInputActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpInputActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Project_P/Input/Actions/IA_Jump.IA_Jump'"));
	if (JumpInputActionRef.Object)
	{
		JumpAction = JumpInputActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookInputActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Project_P/Input/Actions/IA_Look.IA_Look'"));
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
			//PPGAS_LOG(LogPPGAS, Log, TEXT("ASC is successed"));
			
			ASC->InitAbilityActorInfo(GASPlayerState, this);
		}
	}
}

void APPGASCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	//서브시스템을 가져오기 위해 GetSubsystem 함수를 사용
	//ULocalPlayer* 는 APlayerController*에서 GetLocalPlayer()로 가져옴
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		//서브시스템에서 인풋맵핑컨텍스트를 관리함
		// 같은 입력맵핑컨택스트가 있을시 우선순위가 높은게 실행 됨
		// 언제든지(인게임 라이브 중에도) 맵핑컨택스트를 추가, 제거가 가능함
		//AddMappingContext(인풋맵핑컨택스트, 우선순위)
		//Subsystem->RemoveMappingContext(DefaultMappingContext);
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		
	}
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
	//X는 좌우 Y는 상하 입력
	FVector2D MovementVector = Value.Get<FVector2D>();

	//CameraArm->bUsePawnControlRotation = true; 설정을 통해
	//카메라 암과 컨트롤러의 로테이션은 동기화 됨
	//const FRotator Rotation = Controller->GetControlRotation();
	const FRotator Rotation = CameraArm->GetDesiredRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APPGASCharacterPlayer::Look(const FInputActionValue& Value)
{
	//X는 좌우 Y는 상하 입력
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}
