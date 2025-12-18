## 언리얼 포트폴리오 제작

https://github.com/user-attachments/assets/069e524b-0c41-4e2f-8500-731cfae0d5d6

30fps, 100pktLag 상황에서 동작입니다

<br/>

아래 블로그 링크에서 코드 및 구현 일지를 볼 수 있습니다   
또한 차트링크에선 플로우차트와 클래스 구조도를 볼 수 있습니다.   

<br/>

<a href="https://k99812.tistory.com/category/%ED%8F%AC%ED%8A%B8%ED%8F%B4%EB%A6%AC%EC%98%A4%20%EC%A0%9C%EC%9E%91" height="5" width="10" target="_blank" >
<img src="https://img.shields.io/badge/블로그 링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white">
</a>
<a href="https://www.figma.com/board/mI7YbWfbptJudhKcpAZP3U/Project-P-System-flow?node-id=0-1&t=OYhRpfOBPwjB6KUq-1" height="5" width="10" target="_blank" >
<img src="https://img.shields.io/badge/플로우 차트 (Figma)-000000?style=for-the-badge&logo=figma&logoColor=white">
</a>

<br/>

## 목차

<details>
<summary>📖 목차 보기 (Click to Expand)</summary>

* [개요](#개요)
* [개선 사항](#개선-사항)
* [캐릭터 구조](#Character-구조)
* [입력 처리](#입력처리)
* [콤보 공격](#Combo-Attack)
* [애니메이션](#Animation)
* [AI](#AIController)
* [GameMode](#GameMode)
* [PlayerController](#PlayerController)
* [UI](#UI)

</details>

<br/>

## 개요
| 항목 | 내용 | 비고 |
|------|------|------|
| 개발 기간 | 2024.06 ~ 2025.03 <br> 2025.11 ~  |
| 엔진 | Unreal Engine 5.3.2 |
| 언어 | C++ / Blueprint |
| 주요 시스템 | GAS / AI / UMG / Input <br/> Animation / Network |

## 주요 기능
### 전투 시스템 (GAS 기반)
- 어빌리티, 어트리뷰트셋 기반의 콤보 공격, 공격 판정 설계 및 구현
- 게임 플레이 태그를 활용한 제어 및 상태 관리
- Trace 기반의 공격 판정, 타격 시 GE를 통한 데미지 처리

### 애니메이션 시스템
- Locomotion, Jump, AimOffset, UpperBody Layer 블렌딩 구성
- Montage, AnimNotify 기반 콤보 연계 및 타격 타이밍 처리

### UI 연동
- HP Bar, 데미지 플로팅 텍스트, 플레이어 HUD, 사망/재시작 UI 구현
- Attribute 연동을 통한 UI 실시간 갱신 (Delegate 활용)

### AI 시스템
- Behavior Tree, AIController 기반 추적, 공격, 인식 AI 구현
- AI Perception을 통한 시야, 데미지 기반 인식 로직 설계

### 입력 처리
- Enhanced Input System 기반 입력 및 바인딩
- ENUM을 활용한 유지보수 용이한 설계

<div align="right">
  
[목차로](#전체-프로젝트)

</div>

<br/>

## 개선 사항
아래의 링크를 클릭하면 더 자세한 내용을 볼 수 있습니다.
<br/>

### AnimInstance
* AnimInstance에서 Owner 변수를 PPGASCharacterPlayer에서 ACharacter로 변경 
* AnimInstance에서 PPGASCharacterPlayer를 직접참조하여 델리게이트에 연결하는 로직을 인터페이스를 이용하여 연결하도록 변경
<a href="https://k99812.tistory.com/130" height="5" width="10" target="_blank" >
<img src="https://img.shields.io/badge/블로그 글 링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white">
</a>

<br/>

### Monster AI
* 몬스터의 이동방향으로 회전을 Orient Rotation to Movement로 구현
* 이로인해 몬스터가 정찰할 때 AI의 시야가 몬스터가 바라보는 곳과 일치하지 않음
* 행동트리에 몬스터가 이동 전 목표방향으로 회전하는 테스크를 구현해 해결
<a href="https://k99812.tistory.com/155" height="5" width="10" target="_blank" >
<img src="https://img.shields.io/badge/블로그 글 링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white">
</a>

<br/>

### 콤보공격 개선 및 네트워크 지원
* 기존 Timer를 이용한 공격로직은 클라이언트의 프레임이 불안정하거나 네트워크 렉이
  발생할 경우 클라이언트가 입력을 해도 씹히는 현상 발생
* 공격 로직을 GAS의 AbilityTask와 GameplayTag를 활용하는 GAS 이벤트를 활용하여 개선함
* 또한 네트워크 지원을 위해 기존 GAS 초기화 로직을 개선함
<a href="https://k99812.tistory.com/192" height="5" width="10" target="_blank" >
<img src="https://img.shields.io/badge/블로그 글 링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white">
</a>

### 공격판정 개선
* 기존에 구현하여 사용하던 어빌리티 테스크를   
  언리얼에서 네트워크를 고려하며 설계한 어빌리티 테스크로 교체
* 개선결과로 클라이언트, 서버 두 곳 모두에서 사용할 수 있는 GA 구현
<a href="https://k99812.tistory.com/196" height="5" width="10" target="_blank" >
<img src="https://img.shields.io/badge/블로그 글 링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white">
</a>

<div align="right">
  
[목차로](#전체-프로젝트)

</div>

<br/>


# 기술 설명서

## Character 구조
![image](https://github.com/user-attachments/assets/7e81a2ac-d643-4896-a3df-c9fcce9a7544)


### CharacterBase
플레이어 캐릭터, NPC 캐릭터가 공통적으로 사용하는 함수 및 변수 선언
* Dead 이벤트 관련 함수 및 몽타주
* 콤보공격 관련 변수 및 몽타주
* 캐릭터의 소속을 가진 게임 플레이 태그 관리


### PlayerCharacter
캐릭터의 스탯을 관리하는 어트리뷰트셋은 플레이어 스테이트 클래스에 선언
* 캐릭터의 능력 및 스탯을 관리 해주는 ASC를 생성
* Dead 관련 함수들을 Override
* 사용자 입력처리
* GA 관리 
* 어트리뷰트셋 적용


### NonPlayerCharacter
추후 보스몬스터를 추가할 수 있어 보스몬스터, 잡몹의 공통 함수 및 변수를 선언
* 능력, 스탯 관리 ASC 생성
* 스탯을 저장할 어트리뷰트셋 생성
* Dead 관련 함수 Override

### CharacterGrunt
잡몹 클래스
* 체력 HPBar 위젯 관리
* 어트리뷰트 초기화

<div align="right">
  
[목차로](#전체-프로젝트)

</div>


## 입력처리
![image](https://github.com/user-attachments/assets/8c7fb823-4e61-4d59-81b2-43ccdae2e110)
 
EnhancedInput을 사용
* InputMappintContext에서 InputAction을 맵핑 후 저장

## GA(능력) 처리
GA의 부여는 캐릭터가 빙의될때 호출되는 PossessedBy 함수에서 진행됨

> APPGASCharacterPlayer

    //헤더파일
    UPROPERTY(EditAnywhere, Category = "GAS")
    TArray<TSubclassOf<class UGameplayAbility>> StartAbilites;

    //Cpp파일 PossessedBy 함수
    for (const TSubclassOf<UGameplayAbility>& StartAbility : StartAbilites)
    {
	         //ASC는 직접적으로 GA를 접근, 관리하는게 아닌
	         //FGameplayAbilitySpec 구조체를 통해 간접적으로 관리함
         	 FGameplayAbilitySpec Spec(StartAbility);

         	 ASC->GiveAbility(Spec);
    }
    
![image](https://github.com/user-attachments/assets/2136441d-ce6b-497b-ac62-bdf38ce5952e)

<br/>

* 실행에 입력이 필요없는 GA(Ex 공격판정)들은 TArray를 사용해 저장  
* 캐릭터 블루프린트 클래스를 통해 TArray에 GA를 지정

<br/>

> PPInputEnum.h

 	UENUM(BlueprintType)
	enum class EInputAbility : uint8
	{
		None UMETA(DisplayName = "None"),
		Jump = 10 UMETA(DisplayName = "Jump"),
		Sprint UMETA(DisplayName = "Sprint"),
		LeftAttack UMETA(DisplayName = "LAttack"),
		RightAttack UMETA(DisplayName = "RAttack"),
		Skill UMETA(DisplayName = "Skill")
	};

* 입력 ID로 TMap의 Key로 열거형을 사용하여 Key값을 INT로 사용할 때 보다 직관적임  
* 별도의 헤더파일을 만들어 구현해 다른 클래스에서 GA를 사용할때 해당 헤더를 추가하여 사용 가능

 <br/>

> APPGASCharacterPlayer

    //헤더파일
    UPROPERTY(EditAnywhere, Category = "GAS")
    TMap<EInputAbility, TSubclassOf<class UGameplayAbility>> StartInputAbilites;

    //Cpp파일 PossessedBy 함수
    for (const TPair<EInputAbility, TSubclassOf<class UGameplayAbility>>& StartInputAbility : StartInputAbilites)
    {
		FGameplayAbilitySpec Spec(StartInputAbility.Value);
		Spec.InputID = (int32)StartInputAbility.Key;

		ASC->GiveAbility(Spec);
    }


![image](https://github.com/user-attachments/assets/94a63822-dcae-474e-a31a-ca5538c30a11)

<br/>

* 입력을 통해서 발동되는 GA들은 TMap을 활용해 <열겨형, GA> Key값을 GA를 부여할때 InputID로 지정  

<br/>

> APPGASCharacterPlayer

 	//SetupGASPlayerInputComponent 함수
 	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APPGASCharacterPlayer::GASInputPressed, (int32)EInputAbility::Jump);

* 인풋컴포넌트에 함수를 바인드할 때 열거형을 활용하여 바인드된 함수에 키값을 매개변수로 전달

<br/>

> APPGASCharacterPlayer

	//GASInputPressed 함수
	void APPGASCharacterPlayer::GASInputPressed(int32 InputID)
	{
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

    	//GASInputReleased 함수
     	if (Spec->IsActive())
     	{
     		//어빌리티가 실행중이면 GA의 InputReleased 실행
     		ASC->AbilitySpecInputReleased(*Spec);
     	}

* 전달받은 열거형(키값)을 통해 어빌리티 시스템 컴포넌트(ASC)에서 등록된 GA의 스펙을 가져옴   
* 어빌리티스펙을 통해 GA를 실행 및 취소 등 컨트롤할 수 있음

<div align="right">
  
[목차로](#전체-프로젝트)

</div>

<br/>

## Combo Attack

https://github.com/user-attachments/assets/a3cfa008-0c46-4d4e-8a13-b52bb8c2fbbd

![image](https://github.com/user-attachments/assets/360325b1-1141-4885-94fb-c94afd2047fb)
* GameAbility를 사용해 개발한 콤보공격 플로우 차트
* 개선전 콤보공격 로직

<br/>

<img width="1540" height="998" alt="image" src="https://github.com/user-attachments/assets/8bf4f338-e735-4bd3-9560-e24f55018361" />
<img width="1389" height="573" alt="image" src="https://github.com/user-attachments/assets/a2ad0ee4-041a-4c41-8b1d-6fe629d0c976" />
<img width="1004" height="1082" alt="image" src="https://github.com/user-attachments/assets/de77e60f-0c3b-4af0-8d81-a115c6964730" />
<a href="https://k99812.tistory.com/192" height="5" width="10" target="_blank" >
<img src="https://img.shields.io/badge/블로그 글 링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white">
</a>

* 개선후 콤보공격 로직의 플로우 차트와 네트워크 흐름도
* 네트워크 흐름도에서 (InputOpenEvent, OnInputOpen),   
  (Re-Input, InputPresseed)는 순서가 바뀔 수 있습니다.
* 자세한 내용은 블로그 글을 통해 볼 수 있습니다.

<br/>

## Attack Hit Check
![image](https://github.com/user-attachments/assets/49a357a4-081c-49bc-a447-edef2450c8d8)
* GameAbility, AbilityTask, TargetActor를 사용해 개발한 공격 히트 체크 플로우 차트
* 개선전 공격판정 로직

<br/>

<img width="2872" height="1240" alt="image" src="https://github.com/user-attachments/assets/ea3bb68b-a552-4e35-9fdf-21eada09b9c1" />
<img width="1850" height="1343" alt="image" src="https://github.com/user-attachments/assets/a2ceaaec-1dd2-4e15-9b0e-e1b002341cca" />
<img width="2326" height="717" alt="image" src="https://github.com/user-attachments/assets/2665c359-8563-415f-b4b0-9e1d0b5600cc" />
<a href="https://k99812.tistory.com/196" height="5" width="10" target="_blank" >
<img src="https://img.shields.io/badge/블로그 글 링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white">
</a>

* 기존 구현했던 어빌리티 테스크를 언리얼에서 제공하는   
  UAbilityTask_WaitTargetData 테스크로 변경
* 개선 결과로 클라이언트 예측 구현,
  서버-클라 두 곳 모두에서 쓸 수 있는 GA 구현
* 자세한 내용은 블로그 글을 통해 볼 수 있습니다.

<br/>

### 데미지 이벤트 실행
HitCheck 과정에서 데미지를 주는 액터(가해자) 데미지를 받는 액터(피해자)를 둘다 쉽게   
알 수 있어서 해당 함수에서 데미지 이벤트를 실행시킴

> UPPGA_AttackHitCheck

	//TraceResultCallback->ServerApplyHitLogic
	//ServerApplyHitLogic 함수에서 GameEffect 타겟이 몬스터일 경우 실행
	IGameplayTagAssetInterface* TargetActor = Cast<IGameplayTagAssetInterface>(HitResult.GetActor());
	if (TargetActor && TargetActor->HasMatchingGameplayTag(PPTAG_CHARACTER_MONSTER))
	{
		UAISense_Damage::ReportDamageEvent(this, HitResult.GetActor(), OwnerASC->GetAvatarActor(),
			OwnerAttributeSet->GetAttackRate(), HitResult.GetActor()->GetActorLocation(), HitResult.Location);
	}

* IGameplayTagAssetInterface를 통하여 액터가 몬스터(몬스터 태그)인지 확인
* 몬스터인 경우 UAISense_Damage::ReportDamageEvent 함수를 실행하여 데미지 이벤트 실행

<div align="right">
  
[목차로](#전체-프로젝트)

</div>

<br/>


## Animation
### 전체 AnimGraph
![image](https://github.com/user-attachments/assets/64146ffd-b744-4bdf-a716-b30218c425f7)

### GroundLoco
![image](https://github.com/user-attachments/assets/896eefde-9528-4ecd-965c-958545f0756f)
![image](https://github.com/user-attachments/assets/a0843bb2-cb3e-416a-917e-8d9709ea03db)
   
> Character

	//APPGASCharacterPlayer.h
 	DECLARE_DELEGATE(FInputReleasedDelegate);
 	FInputReleasedDelegate InputReleasedDelegate;

  	//인터페이스를 통해 AnimInstance를 넘겨 받음
  	virtual void BindInputReleasedDelegate(class UPPAnimInstance* InAnimInstance) override;

 	//APPGASCharacterPlayer.cpp
  	void APPGASCharacterPlayer::MoveInputReleased()
	{
		InputReleasedDelegate.Execute();
	}

 	void APPGASCharacterPlayer::BindInputReleasedDelegate(UPPAnimInstance* InAnimInstance)
	{
		InputReleasedDelegate.BindUObject(InAnimInstance, &UPPAnimInstance::SaveLastDirection);
	}

* 움직이는 방향에 맞는 StopAnimation을 실행하기 위해 플레이어 캐릭터에 델리게이트를 생성함
* 플레이어의 입력이 끝나면 델리게이트를 실행하여 마지막으로 움직인 방향을 계산
* AnimInstance 클래스에서 인터페이스를 이용해 캐릭터의 델리게이트에 AnimInstance 클래스의 함수를 바인드


<br/>

> UPPAnimInstance

	//NativeInitializeAnimation
	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		IPPAnimInterface* OwnerInter = Cast<IPPAnimInterface>(Owner);
		if (OwnerInter)
		{
			OwnerInter->BindInputReleasedDelegate(this);
		}
	}

 	//SaveLastDirection
	void UPPAnimInstance::SaveLastDirection()
	{
		LastDirection = CalculateDirection(Velocity , Movement->GetLastUpdateRotation());
	}
   
* NativeInitializeAnimation 함수에서 인터페이스를 통해   
  자기자신(AnimInstance)을 넘겨 델리게이트에 콜백함수를 바인드
  
* SaveLastDirection함수가 델리게이트로 호출되면 CalculateDirection함수로  
  Velocity(캐릭터가 움직이는 방향), GetLastUpdateRotation을 넘겨 Direction을 계산

<br/>
   
### GroundLoco + JumpLoco
![image](https://github.com/user-attachments/assets/fd1f9363-7740-4631-9f4e-d5b65d4997f4)

<br/>

### Loco + AimOffset
![image](https://github.com/user-attachments/assets/ec6ac10b-70c4-471b-8f8e-c997652cf305)  

* UPPAnimInstance 에서 AimRotation - ActorRotation으로 보고있는 방향의 로테이션을 계산하여 AimOffset을 적용

> UPPAnimInstance

	FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation);
 	Roll = DeltaRotation.Roll;
	Yaw = DeltaRotation.Yaw;
	Pitch = DeltaRotation.Pitch;

<br/>

### UpperBody
![image](https://github.com/user-attachments/assets/ca2b6773-7c4c-440b-b480-87c9be78478f)
* UpperBody 슬롯 애니메이션 몽타주를 실행 및 저장
* Layered blend per bone을 통해 특정 bone을 기준으로 UpperBody 몽타주 실행시 기존 애니메이션과 혼합되어 재생

<div align="right">
  
[목차로](#전체-프로젝트)

</div>


<br/>


## AIController
https://github.com/user-attachments/assets/304a73f8-e93c-4e49-8669-28b2bcbe6248
![image](https://github.com/user-attachments/assets/37c90fd0-c08d-4018-9a61-3e35d7d1be04)  
<br/>
AI컨트롤러에서 AI Perception 기능을 이용해 적을 인식 하여 블랙보드에 저장   
AI가 적을 인식할때 델리게이트를 이용하여 몬스터의 HPBar를 컨트롤
* AISense 관리
* 블랙보드, 행동트리 관리
* AIPerception 이벤트 처리

<br/>

> APPAIController

	//생성자
 	APPAIController::APPAIController()
	{
 		~~~
   
   		// Tick
		PrimaryActorTick.bCanEverTick = true;
		//AActor::SetActorTickEnabled() 함수로 조절
		PrimaryActorTick.bStartWithTickEnabled = false;
  
		// AI Perception 설정
		AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
		SetPerceptionComponent(*AIPerceptionComp);

		// Sight Config
		SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfig_Sight"));

		//SightConfig 변수 초기화
  		SenseConfig_Sight->SightRadius = GruntAIData->SightRadius;
    
		~~~
  
		//시야 센스 등록
		SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfig_Sight"));
		AIPerceptionComp->ConfigureSense(*SenseConfig_Sight);
		AIPerceptionComp->SetDominantSense(SenseConfig_Sight->GetSenseImplementation());

		//데미지 센스 등록
		SenseConfig_Damage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("SenseConfig_Damage"));
		SenseConfig_Damage->SetMaxAge(GruntAIData->AISenseAge);
		AIPerceptionComp->ConfigureSense(*SenseConfig_Damage);

		~~~

		//AI 인식, 잊힘 이벤트 델리게이트에 콜백함수 바인드
  		AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &APPAIController::ActorPerceptionUpdated);
		AIPerceptionComp->OnTargetPerceptionForgotten.AddDynamic(this, &APPAIController::ActorPerceptionForgetUpdated);
	}

* bStartWithTickEnabled = false 로 틱함수를 비활성화   
* SetActorTickEnabled 함수를 이용하여 AI가 캐릭터를 인식하였을때만 틱함수 실행   

<br/>
     
> APPAIController

	//Tick
	void APPAIController::Tick(float DeltaTime)
	{
		Super::Tick(DeltaTime);

		ResetTarget();
	}

	//ResetTarget 함수
	//플레이어가 죽었는지 확인
 	void APPAIController::ResetTarget()
	{
		~~~
  
		FGameplayTagContainer Tag(PPTAG_CHARACTER_ISDEAD);
		if (ASC->HasAnyMatchingGameplayTags(Tag))
		{
			GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
			AActor::SetActorTickEnabled(false);
			FindTargetDelegate.ExecuteIfBound(false);
		}
  
  		~~~
	}

* ResetTarget 함수에선 캐릭터의 DeadTag를 확인하여 캐릭터의 죽음을 확인   
* 틱함수 비활성화   
* FindTargetDelegate(콜백함수에서 몬스터의 HPBar 비활성화) 실행   

<br/>
   
> APPAIController

	//AI 인식 업데이트
	void APPAIController::ActorPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
	{
		APawn* PerceptionedPawn = Cast<APawn>(Actor);

		if (PerceptionedPawn && PerceptionedPawn->GetController()->IsPlayerController())
		{
			TSubclassOf<UAISense> SensedStimulsClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus);

			if (SensedStimulsClass == UAISense_Sight::StaticClass())
			{
				PerceptionSensedSight(PerceptionedPawn);
			}

			if (SensedStimulsClass == UAISense_Hearing::StaticClass())
			{
				PerceptionSensedHearing(PerceptionedPawn);
			}
		}
	}

* Actor 변수는 AI가 감각을 통해 인식한 액터
* Stimulus변수에 AI의 어떤 감각으로 함수가 호출됐는지 정보가 들어옴   
* GetSenseClass 함수로 클래스를 가져와 클래스에 맞는 함수를 호출   

<br/>

https://github.com/user-attachments/assets/03e28172-f83d-4ee7-a1cf-95480447eeb4
	
> APPAIController

	//PerceptionSensedSight
	//AI가 시야를 통해 인식했을때 실행
	void APPAIController::PerceptionSensedSight(APawn* PerceptionedPawn)
	{
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PerceptionedPawn);
		if (ASC)
		{
			GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, PerceptionedPawn);
			AActor::SetActorTickEnabled(true);
			FindTargetDelegate.ExecuteIfBound(true);
		}
	}

* BlackBoard의 타겟변수 업데이트  
* 틱함수 활성화   
* FindTargetDelegate(콜백함수에서 몬스터의 HPBar 활성화) 실행   

<br/>

https://github.com/user-attachments/assets/e814d45d-6242-4d1b-b56a-287e2291645a

> APPAIController

	//PerceptionSensedDamage
	//AI가 데미지를 통해 인식했을때 실행
	void APPAIController::PerceptionSensedDamage(APawn* PerceptionedPawn)
	{
		if (IsValid(PerceptionedPawn))
		{
			GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, PerceptionedPawn);
			AActor::SetActorTickEnabled(true);
			FindTargetDelegate.ExecuteIfBound(true);
		}
	}

* 데미지를 준 액터(가해자)가 매개변수로 전달됨
* 변수가 유효한지 확인 후 블랙보드에 저장 및 기타 로직 실행

<br/>
   
> APPAIController

	//ActorPerceptionForgetUpdated
	//타겟이 시야범위를 벗어나고 일정시간이 지났을때 실행
	void APPAIController::ActorPerceptionForgetUpdated(AActor* Actor)
	{
		APawn* PerceptionedPawn = Cast<APawn>(Actor);

		if (PerceptionedPawn && PerceptionedPawn->GetController()->IsPlayerController())
		{
			APawn* Target = Cast<APawn>(GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
			if (PerceptionedPawn == Target)
			{
				GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
				AActor::SetActorTickEnabled(false);
				FindTargetDelegate.ExecuteIfBound(false);
			}
		}
	}

* 벗어난 액터가 블랙보드의 타겟인지 확인 후 타겟초기화   
* 틱함수 비활성화   
* FindTargetDelegate(콜백함수에서 몬스터의 HPBar 비활성화) 실행   

### 행동트리
![image](https://github.com/user-attachments/assets/949de752-ae9d-4bd9-a897-88b2f072c0cc)

<div align="right">
  
[목차로](#전체-프로젝트)

</div>


<br/>


## GameMode
![image](https://github.com/user-attachments/assets/b17e3b4c-0ee7-4d25-a189-1965e5df05b7)
* 인터페이스를 통해 상속받은 함수들이 실행되면 플레이어 컨트롤러에 전달해 UI를 생성

<br/>

> OnPlayerDead 호출

	void APPGASCharacterPlayer::SetDead()
	{
		Super::SetDead();

		~~~

		IPPGameInterface* IPPGameMode = Cast<IPPGameInterface>(GetWorld()->GetAuthGameMode());
		if (IPPGameMode)
		{
			IPPGameMode->OnPlayerDead();
		}
	}
 
* 플레이어의 SetDead 함수가 호출
* GameMode를 인터페이스로 변환해 게임모드를 직접 참조하지 않고 인터페이스를 통해 간접 참조함
* 인터페이스의 OnPlayerDead 함수를 실행

<br/>

> GameMode::OnPlayerDead

	void APPGameMode::OnPlayerDead()
	{
		APPPlayerController* PlayerController = Cast<APPPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PlayerController)
		{
			PlayerController->GameOver();
		}
	}

* 플레이어 컨트롤러를 가져옴
* 컨트롤러의 GameOver함수를 실행하여 UI 생성

<br/>

> OnTakeDamage 호출

	//APPGASCharacterNonPlayer 클래스(Grunt 부모클래스)
	void APPGASCharacterNonPlayer::PossessedBy(AController* NewController)
	{
		Super::PossessedBy(NewController);

		~~~

		ASC->GetGameplayAttributeValueChangeDelegate(UPPGruntAttributeSet::GetDamageAttribute()).
			AddUObject(this, &APPGASCharacterNonPlayer::TakeDamage);
	}

	//APPGASCharacterGrunt 클래스
	void APPGASCharacterGrunt::TakeDamage()
	{
		Super::TakeDamage(ChangeData);

		if (ChangeData.NewValue > 0)
		{
			IPPGameInterface* IPPGameMode = Cast<IPPGameInterface>(GetWorld()->GetAuthGameMode());
			if (IPPGameMode)
			{
				IPPGameMode->OnTakeDamage(ChangeData.NewValue, GetActorLocation());
			}
		}
	}

* Grunt 부모클래스(다른 몬스터들이 사용할 경우를 고려)의 PossessedBy 함수에서 Damage 어트리뷰트 체인지 델리게이트에 함수 바인드
* TakeDamage 함수가 호출되면 게임모드를 인터페이스로 변환
* OnTakeDamage 함수를 호출하여 매개변수로 받은 데미지 값, 몬스터의 위치를 전달

<br/>

> GameMode::OnTakeDamage

	void APPGameMode::OnTakeDamage(const float& Damage, const FVector& ActorPosition)
	{
		APPPlayerController* PlayerController = Cast<APPPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PlayerController)
		{
			PlayerController->ActorTakedDamage(Damage, ActorPosition);
		}
	}

* 플레이어 컨트롤러를 가져와 Damage, ActorPosition을 넘겨 데미지 UI생성

<br/>


## PlayerController
> APPPlayerController

	void APPPlayerController::BeginPlay()
	{
		Super::BeginPlay();

		SetInputMode(FInputModeGameOnly());

		HUDWidget = CreateWidget<UPPHUDWidget>(this, HUDWidgetClass);
			if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}

* 인풋모드 관리
* 플레이어 HUD 생성

<br/>

> APPPlayerController

	void APPPlayerController::GameOver()
	{
		GameOverUIWidget = CreateWidget<UPPGameOverUserWidget>(this, GameOverUIClass);
		if (GameOverUIWidget)
		{
			GameOverUIWidget->AddToViewport();
			EnableInput(this);
			SetShowMouseCursor(true);
		}
	}

* GameOverUI 관리
* 게임모드로 부터 GameOver 함수가 호출되면 UI 생성 및 뷰포트 추가

<br/>

> APPPlayerController.h

	//플레이어컨트롤러 헤더파일
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> DamageUIClass;

	UPROPERTY(VisibleAnywhere, Category = "HUD")
	TArray<TWeakObjectPtr<class UPPFloatingTextUserWidget>> DamageUIArray;

* DamageUI 관리
* DamageUIClass : 생성할 UI를 저장
* DamageUIArray : 생성하고 일정시간후 파괴되는 DamgeUI 특성으로 약참조하는 WeekObjectPtr로 선언
* TQueue 컨테이너가 UPROPERTY를 지원하지 않아 TArray를 이용하여 TQueue를 대체함

<br/>

> APPPlayerController.cpp

	//ActorTakedDamage 함수
	TWeakObjectPtr<UPPFloatingTextUserWidget> DamageUI = CreateWidget<UPPFloatingTextUserWidget>(this, DamageUIClass);
	if (DamageUI.IsValid())
	{
 		DamageUI.Get()->EndLifeTime.BindLambda([&]()
   		{
   			~~~ 
   		});

		//SetTextWidget함수를 먼저 실행뒤 결과에 따라 함수 실행
		if (DamageUI.Get()->SetTextWidget(Damage, ActorPosition))
		{
			DamageUIArray.Emplace(DamageUI.Get());
			DamageUI.Get()->AddToViewport();
		}	
	}

* DamageUI 생성 후 SetTextWidget 함수 실행
* 생성한 UI를 Array에 추가 후 뷰포트에 추가

<div align="right">
  
[목차로](#전체-프로젝트)

</div>


<br/>


## UI
### 몬스터 HP BAR
![image](https://github.com/user-attachments/assets/e2bf30e5-bd5e-44be-a264-ae9723ed376a)
<br/>

### WidgetComponent
> UPPGASWidgetComponent.cpp

	//InitWidget 함수
	UPPGASUserWidget* GASUserWidget = Cast<UPPGASUserWidget>(GetWidget());
	if (GASUserWidget)
	{
		GASUserWidget->SetAbilitySystemComponent(GetOwner());
	}

* WidgetComponent에서 위젯 컴포넌트가 초기화 될때 SetAbilitySystemComponent 함수에 오너를 전달
* InitWidget 함수에서 생성한 위젯을 가져와 오너를 넘겨줌

### PPGASUserWidget
> UPPGASUserWidget 

	//헤더파일
	virtual void SetAbilitySystemComponent(AActor* Owner);
 
	//Cpp파일
	void UPPGASUserWidget::SetAbilitySystemComponent(AActor* Owner)
	{
	    if (IsValid(Owner))
	    {
	        ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
	    }
	}

* 위젯 컴포넌트를 부모로 위젯을 생성하면 위젯에서 제공하는 GetOwningPlayer 함수를 사용할 수 없음   
* 그러므로 부모 위젯 클래스를 생성하여 오너를 받아올 수 있는 함수를 생성해야 됨
* UPPGASUserWidget을 상속받는 클래스에서 재정의 할 수 있게 가상함수로 선언

### PPGASHPBarUserWidget
> UPPGASHpBarUserWidget.cpp

	void UPPGASHpBarUserWidget::SetAbilitySystemComponent(AActor* Owner)
	{
		Super::SetAbilitySystemComponent(Owner);

		if (ASC)
		{
			//특정 어트리뷰트값이 바뀔때 마다 호출되는 델리게이트
			ASC->GetGameplayAttributeValueChangeDelegate(UPPCharacterAttributeSet::GetHealthAttribute()).
				AddUObject(this, &UPPGASHpBarUserWidget::OnHealthAttributeChange);
			ASC->GetGameplayAttributeValueChangeDelegate(UPPCharacterAttributeSet::GetMaxHealthAttribute()).
				AddUObject(this, &UPPGASHpBarUserWidget::OnMaxHealthAttributeChange);
	
			~~~
		}
	}
 
* 부모함수를 호출해 ASC를 가져옴
* SetAbilitySystemComponent 함수에서 매개변수로 들어온 오너를 이용하여 ASC에 어트리뷰트 체인지 델리게이트에 함수 등록
* 프로그래스바, 텍스트 박스 관리

### Player HUD
![image](https://github.com/user-attachments/assets/bd804c33-c2a1-4105-b9d7-aad5f35e5fa1)
<br/>

### PPHUDWidget
* 생성한 위젯들을 관리할 클래스
* PPPlayerStatBarUserWidget 생성

### PPPlayerStatBarUserWidget
* ASC를 통해 어트리뷰트 체인지 델리게이트를 통해 콜백함수 연결
* 프로그래스바, 텍스트박스 생성

## 재시작 UI

https://github.com/user-attachments/assets/e9e4e0ee-f90a-4545-8b29-fa12db39658c

![image](https://github.com/user-attachments/assets/796bd9f7-2586-4520-bc19-a23bb26e9eab)
<br/>

1. 캐릭터 사망시 SetDead 함수가 실행
2. GameMode를 가져와 상속받은 인터페이스로 캐스팅하여 함수 실행
3. 게임모드에서 PlayerController 함수를 실행하여 UI 생성

<br/>

> UPPGameOverUserWidget

 	//헤더파일
  	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "BtnEventGameRestartCpp"))
	void BtnEventGameRestart();

![image](https://github.com/user-attachments/assets/7a643519-adc4-44aa-b12b-abd0c138ba32)

 * 버튼 OnClicked 이벤트 콜백 함수를 BluprintCallable 설정을 해 블루프린트에서 함수 바인드

> UPPGameOverUserWidget
	
  	//cpp파일
   	//BtnEventGameRestart
	void UPPGameOverUserWidget::BtnEventGameRestart()
	{
		APlayerController* OwingPlayerController = GetOwningPlayer();
	
		if (OwingPlayerController)
		{
			//Level restart
			UGameplayStatics::OpenLevel(GetWorld(), GetWorld()->GetFName());
	
			//사용자 입력 비활성화
			OwingPlayerController->SetShowMouseCursor(false);
			OwingPlayerController->DisableInput(OwingPlayerController);
	
			//함수실행시 해당 위젯 제거
			RemoveFromParent();
		}
	}

 * 플레이어 컨트롤러를 통해 입력을 비활성화 및 현재 레벨 재시작
 * RemoveFromParent를 호출해 위젯 제거

<br/>

## Damage UI

https://github.com/user-attachments/assets/7e05d46d-074b-4ccf-9e8e-c709ea7f9647

![image](https://github.com/user-attachments/assets/064dbf8b-7813-4201-8201-5f78d2bfd78a)

1. 데미지 어트리뷰트가 바뀌면 게임모드를 가져와 상속받은 인터페이스로 캐스팅하여 함수실행
2. 게임모드에서 플레이어컨트롤러 함수 실행
3. 플레이어 컨트롤러에서 데미지 UI 생성 및 관리
4. AddViewport 함수 실행전 SetTextWidget 함수를 실행하여 위젯의 위치, 텍스트를 설정

<br/>

![image](https://github.com/user-attachments/assets/833e316f-0f92-4484-b664-8a816dc02c2f)

>APPPlayerController.cpp

	//ActorTakedDamage 함수
	//DamageUI의 델리게이트에 바인드되는 람다 함수
	DamageUI.Get()->EndLifeTime.BindLambda([&]()
	{
		TWeakObjectPtr<UPPFloatingTextUserWidget> TempDamageUI = DamageUIArray[0];

		if (TempDamageUI.IsValid())
		{
			TempDamageUI.Get()->RemoveFromParent();
		}
			
		DamageUIArray.RemoveAt(0);
	});

1. 플레이어컨트롤러에서 SetTextWidget 함수 실행
2. DamageUI에서 플레이어 컨트롤러를 가져와 3D좌표를 뷰표트 좌표로 변환 후 데미지 텍스트 설정
3. 플레이어컨트롤러에서 DamageUI 생성후 뷰포트에 추가되면 NativeConstruct 실행
4. Fade 애니메이션 Finished 델리게이트에 AnimationFinished 함수 바인드 후 UI 포지션 설정 및 애니메이션 재생
5. AnimationFinished 함수 호출이 되면 바인드된 람다 함수 실행

<div align="right">
  
[목차로](#전체-프로젝트)

</div>

## 10 FPS, 제한없음

https://github.com/user-attachments/assets/7f20ad76-1a5a-4516-bb73-a703ea676fb4

# 제한없음, 500 pktLag

https://github.com/user-attachments/assets/c0b0fed6-6966-45b5-bd5b-dc073ae2f612

<div align="right">
  
[목차로](#전체-프로젝트)

</div>
