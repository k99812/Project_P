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
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemGlobals.h"
#include "GameplayTagContainer.h"
#include "Tag/PPGameplayTag.h"
#include "Data/PPComboActionData.h"
#include "Attribute/PPCharacterAttributeSet.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Physics/PPCollision.h"

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
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Project_P/Blueprint/Player/BPAB_PPCountess.BPAB_PPCountess_C"));
	if (AnimInstanceClassRef.Class)
	{
		//Set�Լ� ���
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

//�޺� 
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project_P/Animation/Montage/ComboAttack_Montage.ComboAttack_Montage'"));
	if (ComboAttackMontageRef.Object)
	{
		ComboAttackMontage = ComboAttackMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPPComboActionData> ComboActionDataRef(TEXT("/Script/Project_P.PPComboActionData'/Game/Project_P/Data/PlayerComboActionData.PlayerComboActionData'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

//�޽� ����
//ĳ���� �޽��� �ʱ� ��ġ, �����̼ǰ� ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -95.0f), FRotator(0.0f, -90.0f, 0.0f));

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

//GAS ��ǲ ����
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

	static ConstructorHelpers::FObjectFinder<UInputAction> SprintActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Project_P/Input/Actions/IA_Sprint.IA_Sprint'"));
	if (SprintActionRef.Object)
	{
		SprintAction = SprintActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LeftAttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Project_P/Input/Actions/IA_LeftAttack.IA_LeftAttack'"));
	if (LeftAttackActionRef.Object)
	{
		LeftAttackAction = LeftAttackActionRef.Object;
	}

//Dead ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project_P/Animation/Montage/PlayerDead_Montage.PlayerDead_Montage'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

//���� ���� ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage> LevelStartMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project_P/Animation/Montage/Start_Montage.Start_Montage'"));
	if (LevelStartMontageRef.Object)
	{
		LevelStartMontage = LevelStartMontageRef.Object;
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
			ASC->InitAbilityActorInfo(GASPlayerState, this);

			const UPPCharacterAttributeSet* AttributeSet = ASC->GetSet<UPPCharacterAttributeSet>();
			if (AttributeSet)
			{
				AttributeSet->ActorIsDead.AddDynamic(this, &APPGASCharacterPlayer::ActorIsDead);
			}

			//ASC�� Ư���±װ� ����ų� ���ŵǸ� ȣ���ϴ� ��������Ʈ�� �ݹ��Լ� ����
			ASC->RegisterGameplayTagEvent(PPTAG_CHARACTER_ISCC, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APPGASCharacterPlayer::OnCCTagChanged);

			for (const TSubclassOf<UGameplayAbility>& StartAbility : StartAbilites)
			{
				//ASC�� ���������� GA�� ����, �����ϴ°� �ƴ�
				//FGameplayAbilitySpec ����ü�� ���� ���������� ������
				FGameplayAbilitySpec Spec(StartAbility);

				ASC->GiveAbility(Spec);
			}

			for (const TPair<EInputAbility, TSubclassOf<class UGameplayAbility>>& StartInputAbility : StartInputAbilites)
			{
				FGameplayAbilitySpec Spec(StartInputAbility.Value);

				Spec.InputID = (int32)StartInputAbility.Key;

				ASC->GiveAbility(Spec);
			}

			//���������� GA���ε� �Լ��� ����ǵ��� ����
			SetupGASPlayerInputComponent();
		}
	}

	//�ܼ� Ŀ��� �ڵ�� �Է��ϴ� ��
	//ASC �����
	APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
	PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
}

void APPGASCharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

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
	
	//���� ���� ��Ÿ�� ����
	if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(LevelStartMontage))
	{
		GetMesh()->GetAnimInstance()->Montage_Play(LevelStartMontage, 1.0f);
	}
}

void APPGASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//#include "EnhancedInputComponent.h" �߰�
	//#include "EnhancedInputSubsystems.h" �߰�

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

//Move
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APPGASCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APPGASCharacterPlayer::MoveInputReleased);
//Look
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APPGASCharacterPlayer::Look);

	SetupGASPlayerInputComponent();
}

void APPGASCharacterPlayer::SetupGASPlayerInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	//Jump
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APPGASCharacterPlayer::GASInputPressed, (int32)EInputAbility::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APPGASCharacterPlayer::GASInputReleased, (int32)EInputAbility::Jump);

	//Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APPGASCharacterPlayer::GASInputPressed, (int32)EInputAbility::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APPGASCharacterPlayer::GASInputReleased, (int32)EInputAbility::Sprint);
	
	//Attack
		EnhancedInputComponent->BindAction(LeftAttackAction, ETriggerEvent::Triggered, this, &APPGASCharacterPlayer::GASInputPressed, (int32)EInputAbility::LeftAttack);
		EnhancedInputComponent->BindAction(LeftAttackAction, ETriggerEvent::Completed, this, &APPGASCharacterPlayer::GASInputReleased, (int32)EInputAbility::LeftAttack);
	}

}

void APPGASCharacterPlayer::GASInputPressed(int32 InputID)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputID);

	if (Spec)
	{
		Spec->InputPressed = true;

		if (Spec->IsActive())
		{
			//�����Ƽ�� �������̸� GA�� InputPressed �Լ� ����
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			//�����Ƽ Activate ����
			//�����Ƽ�� ���� �� ASC�κ��� GA�� �ٷ�°� Handle�� ���� ��Ʈ��
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void APPGASCharacterPlayer::GASInputReleased(int32 InputID)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputID);

	if (Spec)
	{
		Spec->InputPressed = false;

		if (Spec->IsActive())
		{
			//�����Ƽ�� �������̸� GA�� InputReleased ����
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

void APPGASCharacterPlayer::ActorIsDead()
{
	SetDead();
}

void APPGASCharacterPlayer::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}

	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_NOCOLLISION);
}

void APPGASCharacterPlayer::Move(const FInputActionValue& Value)
{
	//ASC�� �±� �ޱ�
	//#include "GameplayTagContainer.h" �߰�
	FGameplayTagContainer WalkingTagContainer;
	WalkingTagContainer.AddTag(PPTAG_CHARACTER_ISWALKING);

	if (GetCharacterMovement()->GetMovementName() == TEXT("Walking"))
	{
		if (!ASC->HasAnyMatchingGameplayTags(WalkingTagContainer))
		{
			ASC->AddLooseGameplayTags(WalkingTagContainer);

			//#include "AbilitySystemGlobals.h" �߰�
			if (UAbilitySystemGlobals::Get().ShouldReplicateActivationOwnedTags())
			{
				ASC->AddReplicatedLooseGameplayTags(WalkingTagContainer);
			}
		}

		//�̵��� ���� �����Ƽ ĵ��
		FGameplayTagContainer CancelAbilityTags(PPTAG_ABILITY_ATTACK);
		ASC->CancelAbilities(&CancelAbilityTags);

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

	if (GetCharacterMovement()->GetMovementName() != TEXT("Walking"))
	{
		RemoveTag(WalkingTagContainer);
	}
	
}

void APPGASCharacterPlayer::Look(const FInputActionValue& Value)
{
	//X�� �¿� Y�� ���� �Է�
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APPGASCharacterPlayer::MoveInputReleased()
{
	FGameplayTagContainer WalkingTagContainer;
	WalkingTagContainer.AddTag(PPTAG_CHARACTER_ISWALKING);

	InputReleasedDelegate.Execute();
	RemoveTag(WalkingTagContainer);

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID((int32)EInputAbility::Sprint);
	if (Spec->IsActive())
	{
		ASC->CancelAbilityHandle(Spec->Handle);
	}
}

void APPGASCharacterPlayer::RemoveTag(const FGameplayTagContainer& RemoveTagContainer)
{
	// ASC�� �±� ����
	if (ASC->HasAnyMatchingGameplayTags(RemoveTagContainer))
	{
		ASC->RemoveLooseGameplayTags(RemoveTagContainer);

		if (UAbilitySystemGlobals::Get().ShouldReplicateActivationOwnedTags())
		{
			ASC->RemoveReplicatedLooseGameplayTags(RemoveTagContainer);
		}
	}
}


void APPGASCharacterPlayer::OnCCTagChanged(const FGameplayTag CallBackTag, int32 NewCount)
{
	UE_LOG(LogTemp, Log, TEXT("Tag TEST"));
}


