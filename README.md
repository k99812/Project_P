# Project_P: 언리얼 엔진 5 GAS 기반 멀티플레이

https://github.com/user-attachments/assets/069e524b-0c41-4e2f-8500-731cfae0d5d6

30fps, 100PktLag 상황에서 동작입니다

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
| 개발 기간 | 2024.06 ~ 2025.03 <br> 2025.11 ~ (진행 중) | 2025.03 (v1.0) <br/> 2025.12 (v2.0 Network Update)|
| 엔진 | Unreal Engine 5.3.2 |
| 언어 | C++ / Blueprint |
| 주요 시스템 | GAS / AI / UMG / Input <br/> Animation / Network |

## 주요 기능
### 전투 시스템 (GAS 기반)
- 어빌리티, 어트리뷰트셋 기반의 콤보 공격, 공격 판정 설계 및 구현
- 게임 플레이 태그를 활용한 제어 및 상태 관리
- Trace 기반의 공격 판정, 타격 시 GE를 통한 데미지 처리

### 네트워크 및 동기화
- 리슨서버 환경에서의 클라이언트-서버 구조 구현
- WaitTargetData 분석을 통한 GAS 기반 클라이언트 예측 적용으로
  네트워크 지연 극복

### 애니메이션 시스템
- Locomotion, Jump, AimOffset, UpperBody Layer 블렌딩 구성
- Montage, AnimNotify 기반 콤보 연계 및 타격 타이밍 처리

### UI 연동
- HP Bar, Damage UI, 플레이어 HUD, 사망/재시작 UI 구현
- Attribute 연동을 통한 UI 실시간 갱신 (Delegate 활용)
- 오브젝트 풀을 이용한 UI 재사용(Damage UI, 사망/재시작 UI)

### AI 시스템
- Behavior Tree, AIController 기반 추적, 공격, 인식 AI 구현
- AI Perception을 통한 시야, 데미지 기반 인식 로직 설계

### 입력 처리
- Enhanced Input System 기반 입력 및 바인딩
- ENUM을 활용한 유지보수 용이한 설계

<div align="right">
  
[목차로](#목차)

</div>

<br/>

## 개선 사항
아래의 링크를 클릭하면 더 자세한 내용을 볼 수 있습니다.
<br/>

### Damage UI 오브젝트 풀 적용
* 전투시 생성되는 Damage UI에 오브젝트 풀을 적용하여 최적화
* WorldSubsystem을 이용하여 오브젝트 풀을 생성
* 플레이어 컨트롤러는 한번만 생성되는 UI를 생성 및 관리
* 여러번 생성되는 UI는 서브시스템에서 생성하여 플레이어 컨트롤러의 역할 분리
<a href="https://k99812.tistory.com/202" height="5" width="10" target="_blank" >
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

<br/>

### 공격판정 개선
* 기존에 구현하여 사용하던 어빌리티 테스크를   
  언리얼에서 네트워크를 고려하며 설계한 어빌리티 테스크로 교체
* 개선결과로 클라이언트, 서버 두 곳 모두에서 사용할 수 있는 GA 구현
<a href="https://k99812.tistory.com/196" height="5" width="10" target="_blank" >
<img src="https://img.shields.io/badge/블로그 글 링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white">
</a>

<br/>

### 10 FPS, 제한없음

https://github.com/user-attachments/assets/7f20ad76-1a5a-4516-bb73-a703ea676fb4

### 제한없음, 500 pktLag

https://github.com/user-attachments/assets/c0b0fed6-6966-45b5-bd5b-dc073ae2f612

<div align="right">
  
[목차로](#목차)

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
* InputMappingContext에서 InputAction을 맵핑 후 저장

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

* 입력을 통해서 발동되는 GA들은 TMap을 활용해 <열거형, GA> Key값을 GA를 부여할때 InputID로 지정  

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
  
[목차로](#목차)

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

* 개선후 콤보공격 로직의 플로우 차트와 네트워크 흐름도
* 네트워크 흐름도에서 (InputOpenEvent, OnInputOpen),   
  (Re-Input, InputPresseed)는 순서가 바뀔 수 있습니다.
* 자세한 내용은 블로그 글을 통해 볼 수 있습니다.

<a href="https://k99812.tistory.com/192" height="5" width="10" target="_blank" >
<img src="https://img.shields.io/badge/블로그 글 링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white">
</a>

<br/>

## Attack Hit Check
![image](https://github.com/user-attachments/assets/49a357a4-081c-49bc-a447-edef2450c8d8)
* GameAbility, AbilityTask, TargetActor를 사용해 개발한 공격 히트 체크 플로우 차트
* 개선전 공격판정 로직

<br/>

<img width="2872" height="1240" alt="image" src="https://github.com/user-attachments/assets/ea3bb68b-a552-4e35-9fdf-21eada09b9c1" />
<img width="1850" height="1343" alt="image" src="https://github.com/user-attachments/assets/a2ceaaec-1dd2-4e15-9b0e-e1b002341cca" />
<img width="2326" height="717" alt="image" src="https://github.com/user-attachments/assets/2665c359-8563-415f-b4b0-9e1d0b5600cc" />

* 기존 구현했던 어빌리티 테스크를 언리얼에서 제공하는   
  UAbilityTask_WaitTargetData 테스크로 변경
* 개선 결과로 클라이언트 예측 구현,
  서버-클라 두 곳 모두에서 쓸 수 있는 GA 구현
* 자세한 내용은 블로그 글을 통해 볼 수 있습니다.

<a href="https://k99812.tistory.com/196" height="5" width="10" target="_blank" >
<img src="https://img.shields.io/badge/블로그 글 링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white">
</a>

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
  
[목차로](#목차)

</div>

<br/>


## Animation
### 전체 AnimGraph
![image](https://github.com/user-attachments/assets/64146ffd-b744-4bdf-a716-b30218c425f7)

### GroundLoco
![image](https://github.com/user-attachments/assets/896eefde-9528-4ecd-965c-958545f0756f)
![image](https://github.com/user-attachments/assets/a0843bb2-cb3e-416a-917e-8d9709ea03db)
   
> 델리게이트 바인드

	//PPPlayerCharacterInterface
	DECLARE_DELEGATE(FInputReleasedDelegate);
	virtual FInputReleasedDelegate& GetInputReleasedDelegate() = 0;
	
	//APPGASCharacterPlayer
 	FInputReleasedDelegate InputReleasedDelegate;
  	FORCEINLINE virtual FInputReleasedDelegate& GetInputReleasedDelegate() override { return InputReleasedDelegate; }

  	void APPGASCharacterPlayer::MoveInputReleased()
	{
		InputReleasedDelegate.Execute();
	}

	//UPPAnimInstance
 	IPPPlayerCharacterInterface* Player = Cast<IPPPlayerCharacterInterface>(Owner);
	if (Player)
	{
		Player->GetInputReleasedDelegate().BindUObject(this, &UPPAnimInstance::SaveLastDirection);
	}

* 움직이는 방향에 맞는 StopAnimation을 실행하기 위해 플레이어 캐릭터에 델리게이트를 사용
* 플레이어의 입력이 끝나면 델리게이트를 실행하여 마지막으로 움직인 방향을 계산
* 인터페이스에 델리게이트를 정의하여 인터페이스를 상속받은 캐릭터,   
  cpp에서 참조하는 애님인스턴스가 델리게이트를 알 수 있음
	- 기존에는 캐릭터에 델리게이트를 정의하여 애님인스턴스에선 델리게이트를 알 수 없었음
* 인터페이스 함수를 통해 델리게이트 참조를 반환하여 애님인스턴스에서 직접 콜백함수를 바인드


<br/>

> UPPAnimInstance

 	//SaveLastDirection
	void UPPAnimInstance::SaveLastDirection()
	{
		LastDirection = CalculateDirection(Velocity , Movement->GetLastUpdateRotation());
	}
   
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
  
[목차로](#목차)

</div>


<br/>


## AIController

https://github.com/user-attachments/assets/03e28172-f83d-4ee7-a1cf-95480447eeb4

https://github.com/user-attachments/assets/ed1ab653-93b2-4ec6-8e10-531416f6a213

![image](https://github.com/user-attachments/assets/37c90fd0-c08d-4018-9a61-3e35d7d1be04)  
<br/>
AI컨트롤러에서 AI Perception 기능을 이용해 적을 인식 하여 블랙보드에 저장   
AI가 적을 인식할때 델리게이트를 이용하여 몬스터의 HPBar를 컨트롤
* AISense 관리
* 블랙보드, 행동트리 관리
* AIPerception 이벤트 처리

<br/>

<img width="2049" height="1336" alt="image" src="https://github.com/user-attachments/assets/413c6618-70c9-4063-92fc-978640d72f47" />

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
			UAbilitySystemComponent* ASC = PerceptionedPawn ? UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PerceptionedPawn) : nullptr;

			if (ASC && ASC->HasMatchingGameplayTag(PPTAG_CHARACTER_ISDEAD)) return;

			TSubclassOf<UAISense> SensedStimulsClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus);

			if (SensedStimulsClass == UAISense_Sight::StaticClass())
			{
				PerceptionSensedSight(PerceptionedPawn);
			}	

			if (SensedStimulsClass == UAISense_Hearing::StaticClass())
			{
				if (Stimulus.Strength >= GruntAIData->HearingLoudness)
				{
					PerceptionSensedHearing(PerceptionedPawn, Stimulus.StimulusLocation);
				}
			}

			if (SensedStimulsClass == UAISense_Damage::StaticClass())
			{
				PerceptionSensedDamage(PerceptionedPawn);
			}
		}
	}

https://github.com/user-attachments/assets/304a73f8-e93c-4e49-8669-28b2bcbe6248

* Actor 변수는 AI가 감각을 통해 인식한 액터
* Stimulus변수에 AI의 어떤 감각으로 함수가 호출됐는지 정보가 들어옴   
* GetSenseClass 함수로 클래스를 가져와 클래스에 맞는 함수를 호출
* GameplayTag를 확인하여 캐릭터가 죽은상태면 return
* 시각, 데미지는 바로 타겟을 인식
* 청각은 바로 타겟을 인식하지 않고 타겟 위치로 이동

<br/>
	
> APPAIController

	//PerceptionSensedSight
	//AI가 시야를 통해 인식했을때 실행
	void APPAIController::PerceptionSensedSight(APawn* PerceptionedPawn)
	{
		UE_LOG(LogTemp, Log, TEXT("ActorPerceptionUpdated : %s"), *PerceptionedPawn->GetName());

		if (IsValid(PerceptionedPawn))
		{
			BlackboardTargetUpdate(PerceptionedPawn);
		}
	}  

* 인식한 액터를 BlackboardTargetUpdate 함수로 블랙보드에 저장

<br/>

> APPAIController

	//PerceptionSensedDamage
	//AI가 데미지를 통해 인식했을때 실행
	void APPAIController::PerceptionSensedDamage(APawn* PerceptionedPawn)
	{
		UE_LOG(LogTemp, Log, TEXT("Perception Sensed by Damage : %s"), *PerceptionedPawn->GetName())

		if (IsValid(PerceptionedPawn))
		{
			BlackboardTargetUpdate(PerceptionedPawn);
		}
	}

https://github.com/user-attachments/assets/e814d45d-6242-4d1b-b56a-287e2291645a

* 데미지를 준 액터(가해자)가 매개변수로 전달됨
* 인식한 액터를 BlackboardTargetUpdate 함수로 블랙보드에 저장

<br/>

> APPAIController

	//BlackboardTargetUpdate
	//AI가 시야, 데미지를 통해 인식했을때 실행
	void APPAIController::BlackboardTargetUpdate(APawn* Target)
	{
		if (IsValid(Target) && !IsValid(Blackboard->GetValueAsObject(BBKEY_TARGET)))
		{
			Blackboard->SetValueAsObject(BBKEY_TARGET, Target);
			AActor::SetActorTickEnabled(true);
			FindTargetDelegate.Broadcast(true, Target);
		}
	}

* BlackBoard의 타겟변수 업데이트  
* 틱함수 활성화   
* FindTargetDelegate(콜백함수에서 몬스터의 HPBar 활성화) 실행 

<br/>

> APPAIController

	//PerceptionSensedHearing
	//AI가 청각을 통해 인식했을때 실행
	void APPAIController::PerceptionSensedHearing(APawn* PerceptionedPawn, const FVector& Location)
	{
		UE_LOG(LogTemp, Log, TEXT("Perception Sensed by Hearing : %s"), *PerceptionedPawn->GetName());

		if (Blackboard->GetValueAsObject(BBKEY_TARGET)) return;

		if (IsValid(PerceptionedPawn))
		{
			Blackboard->SetValueAsVector(BBKEY_NOISEPOS, Location);
		}
	}

* 이미 타겟이 존재(전투상황)하면 return하여 이벤트 종료
* BlackBoard의 NoisePos 변수 업데이트

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

<br/>

### 행동트리
<img width="1249" height="699" alt="image" src="https://github.com/user-attachments/assets/5335351e-530e-43eb-ab5b-345cf682f84d" />

* 제일 왼쪽 그룹
몬스터가 플레이어를 인식했을때 작동   
   
* 중앙 그룹
몬스터가 청각 이벤트를 받으면 작동   
MoveTo 노드 실행후 별도의 테스크를 구현하여
NoisePos 블랙보드 키를 초기화함

* 오른쪽 그룹
몬스터가 평상시 정찰할 때 작동

> UBTTask_ClearKey

	EBTNodeResult::Type UBTTask_ClearKey::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
	{
		EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

		if (Result != EBTNodeResult::Succeeded)
		{
			return Result;
		}

		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

		if (Blackboard)
		{
			Blackboard->ClearValue(GetSelectedBlackboardKey());

			return EBTNodeResult::Succeeded;
		}

		return EBTNodeResult::Failed;
	}

<br/>
   
<div align="right">
  
[목차로](#목차)

</div>

<br/>

## 몬스터 HP Bar UI
![image](https://github.com/user-attachments/assets/e2bf30e5-bd5e-44be-a264-ae9723ed376a)

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
* 해당 클래스를 사용하여 몬스터 HP Bar를 생성
  
### 몬스터 HP BAR

<img width="1525" height="965" alt="image" src="https://github.com/user-attachments/assets/f9d4f7af-f5df-47f5-8d62-fffceb4d3f77" />

1. AIPerception 이벤트로 Delegate가 Broadcast
	* Delegate를 통해 인식한 플레이어, bool 변수(인식 했는지)가 넘어간다
   
2. 콜백함수가 실행되면 인터페이스를 통해 간접 참조하여 플레이어 캐릭터의 함수를 실행
	* 몬스터는 자기를 인식한 플레이어 캐릭터를 통해 Client RPC를 보낸다
   
3. 인터페이스를 통해 실행된 함수로 플레이어 캐릭터에서 Client RPC를 호출
4. Client RPC가 실행되면 다시 인터페이스를 통해 몬스터를 간접참조하여 HPBar 조절 함수를 실행
   
코드와 자세한 내용은 아래 블로그 링크를 클릭하면 볼수 있습니다   
<a href="https://k99812.tistory.com/198" height="5" width="10" target="_blank" >
<img src="https://img.shields.io/badge/블로그 글 링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white">
</a>



## Player HUD
![image](https://github.com/user-attachments/assets/bd804c33-c2a1-4105-b9d7-aad5f35e5fa1)

https://github.com/user-attachments/assets/6ed4f5f6-d580-4aa2-a2d8-4efa9fdd69cd

<br/>

### PPHUDWidget
* 생성한 위젯들을 관리할 클래스
* PPPlayerStatBarUserWidget 의 상위 위젯

<br/>

### PPPlayerStatBarUserWidget
<img width="2116" height="1185" alt="image" src="https://github.com/user-attachments/assets/c3f8b949-3793-4112-b968-d9d88d15e202" />

<br/>
   
싱글플레이에선 NativeConstruct에서 어트리뷰트 델리게이트에 바인드해도 문제가 없었지만   
멀티플레이에선 클라이언트의 NativeConstruct 호출 시점에 ASC가 초기화가 마무리되지 않아   
바인드가 정상적으로 되지 않음 따라서 위 흐름도처럼 GAS 초기화 이후 별도의 함수로 초기화를 진행하였다
   
1. ASC 초기화가 마무리 되는 PossessedBy, OnRep_PlayerState에서 인터페이스 함수 실행
2. InitHUD 함수에서 HUD위젯, ASC가 생성이 되어있으면 BindAbilitySystem 함수 실행
   아니면 ASC_Cache로 저장
3. 플레이어 컨트롤러의 BeginPlay 실행 시점에 ASC_Cache가 있으면 BindAbilitySystem 함수 실행

<br/>

### Timer를 이용한 StatBar 보간
HP Bar를 부드럽게 변경하기 위해 Tick 함수대신 Timer를 사용했다   
Tick 대신 Timer를 사용한 이유는 Tick 함수는 스탯 변화가 없어도   
계속 프레임당 실행되기 때문에 Timer를 사용하였다   

<br/>

> UPPPlayerStatBarUserWidget

	UPPPlayerStatBarUserWidget::BindAbilitySystem(UAbilitySystemComponent* ASC)
	{
		생략
		ASC->GetGameplayAttributeValueChangeDelegate(UPPCharacterAttributeSet::GetHealthAttribute()).
			AddUObject(this, &UPPPlayerStatBarUserWidget::OnHealthAttributeChange);
	}
	
  	void UPPPlayerStatBarUserWidget::OnHealthAttributeChange(const FOnAttributeChangeData& ChangeData)
	{
		TargetHealth = ChangeData.NewValue;
		CheckShouldTick();
	}

다른 어트리뷰트도 위와 같이 콜백함수를 연결해준다   
* TargetHealth : 변경된 체력값
* CurrentHealth : 현재의 체력값(보간으로 UI에 반영될 값)

<br/>

> UPPPlayerStatBarUserWidget

	void UPPPlayerStatBarUserWidget::CheckShouldTick()
	{
		bool bHealthMatched = FMath::IsNearlyEqual(CurrentHealth, TargetHealth, 0.1f) &&
			FMath::IsNearlyEqual(CurrentMaxHealth, TargetMaxHealth, 0.1f);

		bool bManaMatched = FMath::IsNearlyEqual(CurrentMana, TargetMana, 0.1f) &&
			FMath::IsNearlyEqual(CurrentMaxMana, TargetMaxMana, 0.1f);

		if (bHealthMatched && bManaMatched)
		{
			if (InterpTimerHandle.IsValid())
			{
				GetWorld()->GetTimerManager().ClearTimer(InterpTimerHandle);
				InterpTimerHandle.Invalidate();
			}
		}
		else
		{
			if (!InterpTimerHandle.IsValid())
			{
				GetWorld()->GetTimerManager().SetTimer(InterpTimerHandle, this, &UPPPlayerStatBarUserWidget::UpdateStatBar, TimerFrequency, true);
			}
		}
	}

* IsNearlyEqual 함수를 이용하여 CurrentHealth와 TargetHealth를 체크한다
* 두 변수가 같을경우(근접) 타이머를 해제한다(주로 타이머 콜백함수 UpdateStatBar에서 다시 호출한경우)
* 두 변수가 같지 않을 경우 타이머가 실행되있지 않으면 타이머를 실행하여 콜백함수를 실행한다
* 콜백함수 주기는 0.016(1/60)으로 설정하였다

<br/>

> UPPPlayerStatBarUserWidget

	void UPPPlayerStatBarUserWidget::UpdateStatBar()
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();

		if (!FMath::IsNearlyEqual(CurrentHealth, TargetHealth, 0.1f) ||
			!FMath::IsNearlyEqual(CurrentMaxHealth, TargetMaxHealth, 0.1f))
		{
			CurrentHealth = FMath::FInterpTo(CurrentHealth, TargetHealth, DeltaTime, BarInterpSpeed);
			CurrentMaxHealth = FMath::FInterpTo(CurrentMaxHealth, TargetMaxHealth, DeltaTime, BarInterpSpeed);
			UpdateHpBar();
		}

		생략

		CheckShouldTick();
	}

* 보간에 필요한 DeltaTime을 월드에서 가져왔다
* IsNearlyEqual 함수를 통해 체크, 보간을 진행한 뒤 UI 반영(UpdateHpBar 함수)
* CheckShouldTick 함수를 실행하여 타이머를 종료할지 결정한다

<br/>

### 재시작 UI
<img width="1970" height="1225" alt="image" src="https://github.com/user-attachments/assets/1fe97d13-4d14-485d-86ba-444e29863b8a" />

<br/>

1. PostGameplayEffectExecute 함수에서 GetHealth() <= 0.0f 로 bIsDead = false로 변경
2. 서버에선 OnRep_IsDead 직접 호출, 클라이언트에선 리플리케이션으로 OnRep_IsDead 함수 호출
3. OnRep_IsDead 함수에서 ActorIsDead 델리게이트 발동 및 ASC에 태그 부착
4. 델리게이트 콜백함수 실행하여 몽타주 재생, 콜리전 비활성화, 인터페이스로 UI 생성 요청
5. 플레이어 컨트롤러에서 로컬플레이어가 아니면 종료
6. 생성된 UI가 없을때만 생성 및 저장
7. 저장된 UI 객체가 있으면 재사용

<br/>

> UPPCharacterAttributeSet

 	//헤더파일
  	UPROPERTY(ReplicatedUsing = OnRep_IsDead)
	bool bIsDead = false;

	void UPPCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
	{
		생략

		if (GetHealth() <= 0.0f && !bIsDead)
		{
			bIsDead = true;
			OnRep_IsDead();
		}
	}

PostGameplayEffectExecute 함수에서 캐릭터 죽음 체크 및 이벤트 발동

<br/>

> APPGASCharacterPlayer

 	void APPGASCharacterPlayer::SetDead()
	{
		Super::SetDead();

		PPNET_LOG(LogGAS, Log, TEXT("Begin"));
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			DisableInput(PlayerController);
		}

		if (GetCapsuleComponent())
		{
			GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_NOCOLLISION);
		}

		IPPPlayerInterface* IPlayerController = Cast<IPPPlayerInterface>(PlayerController);
		if (IPlayerController)
		{
			IPlayerController->OnPlayerDead();
		}
	}

* 해당 함수는 몬스터, 플레이어 캐릭터의 공통 부모 CharacterBase 클래스에서   
  가상함수로 선언하여 기본적인 몽타주 재생을 수행함   
* 캐릭터에선 추가적으로 인풋, 콜리전 비활성화 및 GameOverUI 요청을 함

<br/>

> APPPlayerController

 	void APPPlayerController::GameOver()
	{
		if (!IsLocalPlayerController()) return;

		if (GameOverUIWidget && GameOverUIWidget->IsInViewport()) return;

		if (!GameOverUIWidget && GameOverUIClass)
		{
			GameOverUIWidget = CreateWidget<UPPGameOverUserWidget>(this, GameOverUIClass);
		}
	
		GameOverUIWidget->AddToViewport();

		FInputModeUIOnly UIInputMode;
		UIInputMode.SetWidgetToFocus(GameOverUIWidget->TakeWidget());
		UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		SetInputMode(UIInputMode);
		SetShowMouseCursor(true);
	}

* GameOverUIWidget 인스턴스가 없을때만 생성 및 저장
* 이벤트가 발생되면 저장된 인스턴스를 뷰포트에 추가

<br/>

> UPPGameOverUserWidget

 	void UPPGameOverUserWidget::BtnEventGameRestart()
	{
		생략

		RemoveFromParent();
	}

* 버튼 이벤트가 발생하면 UI를 뷰포트에서 제거

<br/>

> UPPCharacterAttributeSet
	
  	void UPPCharacterAttributeSet::OnRep_IsDead()
	{
		PPNET_ATTLOG(LogGAS, Log, TEXT("Begin"));

		if (bIsDead)
		{
			ActorIsDead.Broadcast();
		}

		UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
		if (ASC)
		{
			if (bIsDead)
			{
				if (!ASC->HasMatchingGameplayTag(PPTAG_CHARACTER_ISDEAD))
				{
					ASC->AddLooseGameplayTag(PPTAG_CHARACTER_ISDEAD);
				}
			}
			else
			{
				if (ASC->HasMatchingGameplayTag(PPTAG_CHARACTER_ISDEAD))
				{
					ASC->RemoveLooseGameplayTag(PPTAG_CHARACTER_ISDEAD);
				}
			}

			AActor* Avartar = ASC->GetAvatarActor();
			if (IPPCharacterBaseInterface* Player = Cast<IPPCharacterBaseInterface>(Avartar))
			{
				if (bIsDead)
				{
					if (Avartar->GetLocalRole() == ENetRole::ROLE_SimulatedProxy)
					{
						Player->SetDead();
					}
				}
				else
				{
					Player->SetAlive();
				}
			}
		}
	}

OnRep_IsDead 함수는 캐릭터 죽음 말고도 부활할 때도 실행됨   
ASC에 캐릭터의 죽음태그를 bIsDead에 따라 부착 또는 제거   
<br/>
IPPCharacterBaseInterface로 실행되는 SetDead, SetAlive 함수들은   
로컬플레이어 말고 SimulatedProxy를 위한 이벤트   
<br/>
로컬플레이어의 죽음 함수는 ActorIsDead로 실행   
부활 함수는 파괴 및 재생성할 때 PossessedBy, OnRep_PlayerState 함수에서 실행

<br/>

### 캐릭터 부활 기능
<img width="2269" height="1208" alt="image" src="https://github.com/user-attachments/assets/1b8d0d73-2ae6-4a1c-b597-c0ebc474f15b" />

1. GameOver UI의 버튼 이벤트가 발생
2. ASC를 통해 클라이언트 예측으로 Tag, 어트리뷰트셋 isDead 변수 초기화
3. 인터페이스를 통해 RequestRespawn 함수(RPC)를 실행하여 서버에 부활요청을 한다
4. 부활 요청을 받은 서버는 죽음 관련 변수를 초기화 및 액터 파괴, 재생성한다
5. isDead 리플리케이션을 통해 캡슐컴포넌트, 무브먼트 관련 초기화를 진행한다 

클라이언트에서 먼저 반영하는 이유는 만약 먼저 반영하지않고 서버의 동기화를   
기다리면 서버에서 동기화가 되기전까지 캐릭터가 부활하면 바로죽는 버그가 생긴다   
<br/>
리플리케이션을 통해 진행해야 시뮬레이티드 프록시에도 정상적으로 적용된다   
멀티캐스트를 사용하지 않은 이유는 부활, 죽음 로직은 플레이어가 중단 or 중간 진입하여   
RPC 이후에 접속하여도 적용되어야 하기 때문이다 

<br/>

> UPPGameOverUserWidget

 	//헤더파일
  	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "BtnEventGameRestartCpp"))
	void BtnEventGameRestart();

![image](https://github.com/user-attachments/assets/7a643519-adc4-44aa-b12b-abd0c138ba32)

 * 버튼 OnClicked 이벤트 콜백 함수를 BluprintCallable 설정을 해 블루프린트에서 함수 바인드

<br/>

> UPPGameOverUserWidget

 	void UPPGameOverUserWidget::BtnEventGameRestart()
	{
		생략
	
		IAbilitySystemInterface* IPlayerState = Cast<IAbilitySystemInterface>(OwningPlayerController->PlayerState);
		UAbilitySystemComponent* ASC = IPlayerState ? IPlayerState->GetAbilitySystemComponent() : nullptr;
		if (ASC)
		{
			if (ASC->HasMatchingGameplayTag(PPTAG_CHARACTER_ISDEAD))
			{
				ASC->RemoveLooseGameplayTag(PPTAG_CHARACTER_ISDEAD);
			}

			if (const UPPCharacterAttributeSet* AttributeSet = ASC->GetSet<UPPCharacterAttributeSet>())
			{
				const_cast<UPPCharacterAttributeSet*>(AttributeSet)->SetIsDead(false);
			}
		}

		IPlayerController->RequestRespawn();

		OwningPlayerController->SetShowMouseCursor(false);
		OwningPlayerController->SetInputMode(FInputModeGameOnly());

		RemoveFromParent();
	}

특정 클래스에 의존하는 것을 방지하기 위해 언리얼의 기반 클래스(APlayerController)와   
인터페이스를 활용하여 객체 간의 결합도를 낮추고 로직의 범용성과 재사용성을 확보   
<br/>
* ASC->RemoveLooseGameplayTag(PPTAG_CHARACTER_ISDEAD);
* const_cast<UPPCharacterAttributeSet*>(AttributeSet)->SetIsDead(false);   
<br/>

클라이언트 예측으로 버튼을 누르는 즉시 죽음 관련 변수, 태그들을 제거   
선반영을 하지않으면 서버에서 리플리케이션 되기 전까지 게임플레이 태그,변수가   
살아있어 캐릭터가 부활하자마자 죽음 관련 이벤트가 계속 실행된다   
<br/>

<br/>

> APPPlayerController

 	void APPPlayerController::RequestRespawn()
	{
		ServerRPC_RequestRespawn();
	}

	void APPPlayerController::ServerRPC_RequestRespawn_Implementation()
	{
		생략
		//서버에서 죽음관련 변수 초기화
		ASC->RemoveLooseGameplayTag(PPTAG_CHARACTER_ISDEAD);
		const_cast<UPPCharacterAttributeSet*>(AttributeSet)->SetIsDead(false);
		
		생략
		UnPossess();
		OldCharacter->Destroy();

		생략
		GameMode->RestartPlayer(this);
	}

코드가 길어 변수 생성, 초기화, 방어코드는 생략하였음   
<br/>
* ServerRPC가 실행되면 서버에서 죽음관련 변수를 초기화
* SetIsDead 함수로 bIsDead가 바뀌면 서버, 로컬클라는 OnRep함수가 실행되지않음
	- 서버는 별도실행안함
	- 로컬클라는 선반영으로 변수가 이미 변경됨
	- 따라서 시뮬레이티드 프록시만 OnRep 함수가 실행됨
* 기존 빙의한 액터를 UnPossess, Destory 진행
* 게임모드를 통해 RestartPlayer 함수로 재생성

<br/>

> APPGASCharacterPlayer

 	void APPGASCharacterPlayer::SetAlive()
	{
		PPNET_LOG(LogGAS, Log, TEXT("Begin"));
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
	}

* 캡슐 컴포넌트, 무브먼트 관련 초기화 진행

<br/>

## Damage UI

https://github.com/user-attachments/assets/7e05d46d-074b-4ccf-9e8e-c709ea7f9647

<img width="1466" height="1114" alt="image" src="https://github.com/user-attachments/assets/e5489741-3ea9-4015-990b-377c26263795" />

<br/>

1. 서버에서 GameEffect로 데미지 적용
2. PostGameplayEffectExecute 에서 데미지 처리 및 이벤트 발동
3. FGameplayEffectContextHandle 을 통해 가해자(타격한 액터)에게 RPC 전송
4. 가해자 액터 클라이언트에서 Damage UI 출력
5. Damage UI의 애니메이션이 끝나면 오브젝트 풀로 객체 반환

<br/>

<img width="2093" height="935" alt="image" src="https://github.com/user-attachments/assets/c32e3d1e-cc6c-458f-a653-7a406ae7067e" />
<img width="2523" height="718" alt="image" src="https://github.com/user-attachments/assets/cfac633d-854a-44fb-b342-62d771def0f6" />
<img width="2495" height="774" alt="image" src="https://github.com/user-attachments/assets/b715be12-02e3-4a1a-8be4-01d3abb281a7" />

<br/>

1. Attack Hit Check GA에서 공격 판정이 일어나 GameEffect 적용(서버)
2. 어트리뷰트셋 PostGameplayEffectExecute 에서 GameEffect 적용 및 Damage UI 생성 요청(서버)
    * 해당 함수에서 GameEffectContext 를 이용해 가해자, 매개변수 Data를 이용해 타겟(피해자)을 구할 수 있음
3. 가해자(공격을 한 액터)의 플레이어 컨트롤러에서 ClientRPC를 통해 오브젝트 풀(월드 서브시스템)에 Damage UI 요청
4. 오브젝트 풀에 UI가 존재하면 풀에 있는 UI를 꺼내고 존재하지 않으면 생성
	* 이때 UI 변수 초기화(위치, Text, 투명도 등)하여 그 전에 기록된 값을 기본 값으로 되돌린다
5. UI 애니메이션이 종료되면 오브젝트 풀에 UI 객체를 저장한다

<br/>

자세한 코드는 아래 블로그 글 링크에 확인할 수 있습니다   
<a href="https://k99812.tistory.com/202" height="5" width="10" target="_blank" >
<img src="https://img.shields.io/badge/블로그 글 링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white">
</a>


<div align="right">
  
[목차로](#목차)

</div>

# 현재는 사용하지 않음
싱글플레이에서 멀티플레이로 리팩토링하여     
현재는 사용하지 않는 로직들

<details>

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
* DamageUIArray : 생성하고 일정시간후 파괴되는 DamgeUI 특성으로 약참조하는 WeakObjectPtr로 선언
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

</details>

<div align="right">
  
[목차로](#목차)

</div>
