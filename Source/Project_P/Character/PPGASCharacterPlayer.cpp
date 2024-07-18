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
//���̷��ڸ޽� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonCountess/Characters/Heroes/Countess/Meshes/SM_Countess.SM_Countess'"));
	if (CharacterMeshRef.Object)
	{
		//(SkeletalMesh = ����)�� �ƴ� SetSkeletalMesh(����)�� �ؾ���
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

//�ִϸ��̼��ν��Ͻ�Ŭ���� ����
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Project_P/Blueprint/BP_PPABCountess.BP_PPABCountess_C"));
	if (AnimInstanceClassRef.Class)
	{
		//Set�Լ� ���
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

//ĳ���� �����Ʈ ����
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 2000.0f, 0.0f);

//ī�޶� ��(USpringArmComponent) ����
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	//ĳ���Ϳ� ī�޶� ���� �Ÿ�
	CameraArm->TargetArmLength = 300.0f;
	//�����̼��� ��Ʈ�ѷ��� �����̼ǰ� ����ȭ ����
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));

//ī�޶� ����(UCameraComponent)
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//ī�޶� ī�޶�Ͽ� �ڽ����� ����
	FollowCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	//�����̼��� ��Ʈ�ѷ��� �����̼ǰ� ����ȭ ����
	FollowCamera->bUsePawnControlRotation = false;

//��ǲ ����
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

	//����ý����� �������� ���� GetSubsystem �Լ��� ���
	//ULocalPlayer* �� APlayerController*���� GetLocalPlayer()�� ������
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		//����ý��ۿ��� ��ǲ�������ؽ�Ʈ�� ������
		// ���� �Է¸������ý�Ʈ�� ������ �켱������ ������ ���� ��
		// ��������(�ΰ��� ���̺� �߿���) �������ý�Ʈ�� �߰�, ���Ű� ������
		//AddMappingContext(��ǲ�������ý�Ʈ, �켱����)
		//Subsystem->RemoveMappingContext(DefaultMappingContext);
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		
	}
}

void APPGASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//#include "EnhancedInputComponent.h" �߰�
	//#include "EnhancedInputSubsystems.h" �߰�

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APPGASCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APPGASCharacterPlayer::Look);
}

void APPGASCharacterPlayer::Move(const FInputActionValue& Value)
{
	//X�� �¿� Y�� ���� �Է�
	FVector2D MovementVector = Value.Get<FVector2D>();

	//CameraArm->bUsePawnControlRotation = true; ������ ����
	//ī�޶� �ϰ� ��Ʈ�ѷ��� �����̼��� ����ȭ ��
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
	//X�� �¿� Y�� ���� �Է�
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}
