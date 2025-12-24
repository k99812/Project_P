// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPGASCharacterPlayer.h"
#include "Project_P.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemGlobals.h"
#include "Attribute/PPCharacterAttributeSet.h"
#include "GameplayTagContainer.h"
#include "Tag/PPGameplayTag.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/PPAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Data/PPComboActionData.h"
#include "Physics/PPCollision.h"
#include "Perception/AISense_Hearing.h"
#include "UI/PPGASWidgetComponent.h"
#include "Project_P.h"
#include "Interface/PPGameInterface.h"
#include "Interface/PPPlayerInterface.h"
#include "Interface/PPMonsterInterface.h"
#include "Interface/PPPlayerStateInterface.h"

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
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Project_P/Blueprint/Player/BPAB_PPCountess.BPAB_PPCountess_C"));
	if (AnimInstanceClassRef.Class)
	{
		//Set함수 사용
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

//콤보 
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

//메쉬 설정
//캐릭터 메쉬의 초기 위치, 로테이션값 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -95.0f), FRotator(0.0f, -90.0f, 0.0f));

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

//GAS 인풋 설정
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

//Dead 몽타주 설정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project_P/Animation/Montage/PlayerDead_Montage.PlayerDead_Montage'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

//레벨 시작 몽타주 설정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> LevelStartMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project_P/Animation/Montage/Start_Montage.Start_Montage'"));
	if (LevelStartMontageRef.Object)
	{
		LevelStartMontage = LevelStartMontageRef.Object;
	}

//Team
	SetGenericTeamId(FGenericTeamId(ETeamAttitude::Hostile));
}

UAbilitySystemComponent* APPGASCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void APPGASCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitGAS();

	//콘솔 커멘드 코드로 입력하는 법
	//ASC 디버그
	APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
	PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));

	IPPPlayerInterface* Player = Cast<IPPPlayerInterface>(PlayerController);
	if (Player)
	{
		Player->InitHUD(ASC);
	}
}

void APPGASCharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitGAS();

	IPPPlayerInterface* Player = Cast<IPPPlayerInterface>(GetController());
	if (Player)
	{
		Player->InitHUD(ASC);
	}
}

void APPGASCharacterPlayer::InitGAS()
{
	APlayerState* PState = GetPlayerState();
	IAbilitySystemInterface* GASPlayerState = PState ? Cast<IAbilitySystemInterface>(PState) : nullptr;
	if (GASPlayerState)
	{
		ASC = GASPlayerState->GetAbilitySystemComponent();
		if (ASC)
		{
			ASC->InitAbilityActorInfo(PState, this);
			ASC->SetIsReplicated(true);

			const UPPCharacterAttributeSet* AttributeSet = ASC->GetSet<UPPCharacterAttributeSet>();
			if (AttributeSet)
			{
				AttributeSet->ActorIsDead.AddDynamic(this, &APPGASCharacterPlayer::ActorIsDead);

				if (AttributeSet->GetIsDead())
				{
					ActorIsDead();
				}
				else
				{
					SetAlive();
				}
			}

			//ASC에 특정태그가 생기거나 제거되면 호출하는 델리게이트에 콜백함수 연결
			//ASC->RegisterGameplayTagEvent(PPTAG_CHARACTER_ISCC, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APPGASCharacterPlayer::OnCCTagChanged);

			IPPPlayerStateInterface* IPlayerState = PState ? Cast<IPPPlayerStateInterface>(PState) : nullptr;
			if (HasAuthority() && IPlayerState)
			{
				if (!IPlayerState->GetIsAbilitiesGiven())
				{
					for (const TSubclassOf<UGameplayAbility>& StartAbility : StartAbilites)
					{
						//ASC는 직접적으로 GA를 접근, 관리하는게 아닌
						//FGameplayAbilitySpec 구조체를 통해 간접적으로 관리함
						FGameplayAbilitySpec Spec(StartAbility);

						ASC->GiveAbility(Spec);
					}

					for (const TPair<EInputAbility, TSubclassOf<class UGameplayAbility>>& StartInputAbility : StartInputAbilites)
					{
						FGameplayAbilitySpec Spec(StartInputAbility.Value);

						Spec.InputID = (int32)StartInputAbility.Key;

						ASC->GiveAbility(Spec);
					}

					IPlayerState->SetIsAbilitiesGiven(true);
				}

				InitializeAttributes();
			}
		}
	}
}

void APPGASCharacterPlayer::InitializeAttributes()
{
	if (ASC && DefaultStatsEffect)
	{
		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		ContextHandle.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DefaultStatsEffect, 1.0f, ContextHandle);

		if (SpecHandle.IsValid())
		{
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}

		if (ASC->HasMatchingGameplayTag(PPTAG_CHARACTER_ISDEAD))
		{
			ASC->RemoveLooseGameplayTag(PPTAG_CHARACTER_ISDEAD);
		}

		if (const UPPCharacterAttributeSet* Attribute = ASC->GetSet<UPPCharacterAttributeSet>())
		{
			UPPCharacterAttributeSet* MutableAttribute = const_cast<UPPCharacterAttributeSet*>(Attribute);
			MutableAttribute->SetIsDead(false);
		}
	}
}

void APPGASCharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void APPGASCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//레벨 시작 몽타주 실행
	if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(LevelStartMontage))
	{
		GetMesh()->GetAnimInstance()->Montage_Play(LevelStartMontage, 1.0f);
	}
}

void APPGASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//#include "EnhancedInputComponent.h" 추가
	//#include "EnhancedInputSubsystems.h" 추가

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

//Move
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APPGASCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APPGASCharacterPlayer::MoveInputReleased);
//Look
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APPGASCharacterPlayer::Look);

	//서브시스템을 가져오기 위해 GetSubsystem 함수를 사용
	//ULocalPlayer* 는 APlayerController*에서 GetLocalPlayer()로 가져옴
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (PlayerController)
	{
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

	SetupGASPlayerInputComponent();
}

void APPGASCharacterPlayer::RequestSetMonsterHpBar(bool bIsFound, AActor* TargetMonster)
{
	if (!HasAuthority()) return;

	ClientRPC_SetMonsterHpBar(bIsFound, TargetMonster);
}

void APPGASCharacterPlayer::ClientRPC_SetMonsterHpBar_Implementation(bool bIsFound, AActor* TargetMonster)
{
	IPPMonsterInterface* Target = Cast<IPPMonsterInterface>(TargetMonster);
	if (Target)
	{
		Target->SetMonstHpBarVisibility(bIsFound);
	}
}

void APPGASCharacterPlayer::BindInputReleasedDelegate(UPPAnimInstance* InAnimInstance)
{
	InputReleasedDelegate.BindUObject(InAnimInstance, &UPPAnimInstance::SaveLastDirection);
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
	//UAISense_Hearing::ReportNoiseEvent(this, this->GetActorLocation(), 1.f, this);

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputID);

	if (Spec)
	{
		Spec->InputPressed = true;

		if (Spec->IsActive())
		{
			//어빌리티가 실행중이면 GA의 InputPressed 함수 실행
			ASC->AbilitySpecInputPressed(*Spec); 
		}
		else
		{
			//어빌리티 Activate 실행
			//어빌리티의 실행 등 ASC로부터 GA를 다루는건 Handle을 통해 컨트롤
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
			//어빌리티가 실행중이면 GA의 InputReleased 실행
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

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_NOCOLLISION);
	}

	IPPPlayerInterface* Player = Cast<IPPPlayerInterface>(PlayerController);
	if (Player)
	{
		Player->OnPlayerDead();
	}
}

void APPGASCharacterPlayer::SetAlive()
{
	//PPNET_LOG(LogGAS, Log, TEXT("Begin"));
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PPCAPSULE);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SetActorEnableCollision(true);
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		GetCharacterMovement()->Activate();
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
	}
	
	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
	}
}

void APPGASCharacterPlayer::Move(const FInputActionValue& Value)
{
	//ASC에 태그 달기
	//#include "GameplayTagContainer.h" 추가
	FGameplayTagContainer WalkingTagContainer;
	WalkingTagContainer.AddTag(PPTAG_CHARACTER_ISWALKING);

	if (GetCharacterMovement()->GetMovementName() == TEXT("Walking"))
	{
		if (!ASC->HasAllMatchingGameplayTags(WalkingTagContainer))
		{
			ASC->AddLooseGameplayTags(WalkingTagContainer);

			//#include "AbilitySystemGlobals.h" 추가
			if (UAbilitySystemGlobals::Get().ShouldReplicateActivationOwnedTags())
			{
				ASC->AddReplicatedLooseGameplayTags(WalkingTagContainer);
			}
		}

		//이동시 공격 어빌리티 캔슬
		FGameplayTagContainer CancelAbilityTags(PPTAG_ABILITY_ATTACK);
		ASC->CancelAbilities(&CancelAbilityTags);

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

	if (GetCharacterMovement()->GetMovementName() != TEXT("Walking"))
	{
		RemoveTag(WalkingTagContainer);
	}
	
}

void APPGASCharacterPlayer::Look(const FInputActionValue& Value)
{
	//X는 좌우 Y는 상하 입력
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APPGASCharacterPlayer::MoveInputReleased()
{
	//PPNET_LOG(LogGAS, Log, TEXT("Begin"));
	FGameplayTagContainer WalkingTagContainer;
	WalkingTagContainer.AddTag(PPTAG_CHARACTER_ISWALKING);

	InputReleasedDelegate.Execute();
	RemoveTag(WalkingTagContainer);

	FGameplayTagContainer CancelAbilityTags(PPTAG_ABILITY_SPRINT);
	ASC->CancelAbilities(&CancelAbilityTags);
}

void APPGASCharacterPlayer::RemoveTag(const FGameplayTagContainer& RemoveTagContainer)
{
	// ASC에 태그 제거
	if (ASC->HasAllMatchingGameplayTags(RemoveTagContainer))
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


