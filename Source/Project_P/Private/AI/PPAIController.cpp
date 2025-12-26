// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PPAIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Sight.h"
#include "Data/PPGruntAIData.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagContainer.h"
#include "Tag/PPGameplayTag.h"
#include "AI/PPAI.h"
#include "Character/PPGASCharacterGrunt.h"

APPAIController::APPAIController()
{
	//#include "BehaviorTree/BlackboardData.h"추가
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardRef(TEXT("/Script/AIModule.BlackboardData'/Game/Project_P/AI/BB_PPCharacter.BB_PPCharacter'"));
	if (BlackboardRef.Object)
	{
		BBAsset = BlackboardRef.Object;
	}

	//#include "BehaviorTree/BehaviorTree.h"추가
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Project_P/AI/BT_PPCharacter.BT_PPCharacter'"));
	if(BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPPGruntAIData> AIDataRef(TEXT("/Script/Project_P.PPGruntAIData'/Game/Project_P/Data/GruntAIData.GruntAIData'"));
	if (AIDataRef.Object)
	{
		GruntAIData = AIDataRef.Object;
	}

// Tick
	PrimaryActorTick.bCanEverTick = true;
	//AActor::SetActorTickEnabled() 함수로 조절
	PrimaryActorTick.bStartWithTickEnabled = false;
	
// AI Perception 설정
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	SetPerceptionComponent(*AIPerceptionComp);

// Sight Config
	SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfig_Sight"));

	SenseConfig_Sight->SightRadius = GruntAIData->SightRadius;
	SenseConfig_Sight->LoseSightRadius = GruntAIData->LoseSightRadius;
	SenseConfig_Sight->PeripheralVisionAngleDegrees = GruntAIData->AIVisionAngleDeg;
	//탐지하면 몇초(age)동안 탐지를 유지할지
	SenseConfig_Sight->SetMaxAge(GruntAIData->AISenseAge);
	//마지막으로 감지된 객체의 위치 탐지 성공 - 시야 범위를 벗어난 상태에서도 객체를 일정 거리 이내에서 추적할 수 있다.
	SenseConfig_Sight->AutoSuccessRangeFromLastSeenLocation = GruntAIData->AutoSuccessRange;

	SenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	SenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	SenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;

	AIPerceptionComp->ConfigureSense(*SenseConfig_Sight);
	AIPerceptionComp->SetDominantSense(SenseConfig_Sight->GetSenseImplementation());

// Hearing Config
	SenseConfig_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("SenseConfig_Hearing"));

	SenseConfig_Hearing->HearingRange = GruntAIData->HearingRange;
	SenseConfig_Hearing->SetMaxAge(GruntAIData->AISenseAge);

	SenseConfig_Hearing->DetectionByAffiliation.bDetectEnemies = true;
	SenseConfig_Hearing->DetectionByAffiliation.bDetectFriendlies = false;
	SenseConfig_Hearing->DetectionByAffiliation.bDetectNeutrals = false;
	
	AIPerceptionComp->ConfigureSense(*SenseConfig_Hearing);

// Damage Config
	SenseConfig_Damage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("SenseConfig_Damage"));

	SenseConfig_Damage->SetMaxAge(GruntAIData->AISenseAge);
	AIPerceptionComp->ConfigureSense(*SenseConfig_Damage);

// AI인식 이벤트 델리게이트 바인드
	AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &APPAIController::ActorPerceptionUpdated);
	AIPerceptionComp->OnTargetPerceptionForgotten.AddDynamic(this, &APPAIController::ActorPerceptionForgetUpdated);
}

void APPAIController::RunAI()
{
	//부모 클래스에 UBlackboardComponent가 저장되있음
	UBlackboardComponent* BBComponent = Blackboard.Get();
	if (UseBlackboard(BBAsset, BBComponent))
	{
		//블랙보드 데이터에 값설정
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void APPAIController::StopAI()
{
	//UBehaviorTreeComponent는 BrainComponent의 자식 클래스
	//부모 클래스에 BrainComponent가 선언 되어있음
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void APPAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}

void APPAIController::BeginPlay()
{
	Super::BeginPlay();
}

void APPAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ResetTarget();
}

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

		if (SensedStimulsClass == UAISense_Damage::StaticClass())
		{
			PerceptionSensedDamage(PerceptionedPawn);
		}
	}
}

void APPAIController::ActorPerceptionForgetUpdated(AActor* Actor)
{
	APawn* PerceptionedPawn = Cast<APawn>(Actor);

	if (PerceptionedPawn && PerceptionedPawn->GetController()->IsPlayerController())
	{
		UE_LOG(LogTemp, Log, TEXT("ActorPerceptionForgetUpdated : %s"), *Actor->GetName());

		APawn* Target = Cast<APawn>(GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
		if (PerceptionedPawn == Target)
		{
			FindTargetDelegate.Broadcast(false, Target);
			Blackboard->SetValueAsObject(BBKEY_TARGET, nullptr);
			AActor::SetActorTickEnabled(false);
		}
	}
}

void APPAIController::PerceptionSensedSight(APawn* PerceptionedPawn)
{
	UE_LOG(LogTemp, Log, TEXT("ActorPerceptionUpdated : %s"), *PerceptionedPawn->GetName());

	if (IsValid(PerceptionedPawn))
	{
		BlackboardTargetUpdate(PerceptionedPawn);
	}
}

void APPAIController::PerceptionSensedHearing(APawn* PerceptionedPawn)
{
	UE_LOG(LogTemp, Log, TEXT("Perception Sensed by Hearing : %s"), *PerceptionedPawn->GetName())
}

void APPAIController::PerceptionSensedDamage(APawn* PerceptionedPawn)
{
	UE_LOG(LogTemp, Log, TEXT("Perception Sensed by Damage : %s"), *PerceptionedPawn->GetName())

	if (IsValid(PerceptionedPawn))
	{
		BlackboardTargetUpdate(PerceptionedPawn);
	}
}

void APPAIController::BlackboardTargetUpdate(APawn* Target)
{
	if (IsValid(Target) && !IsValid(Blackboard->GetValueAsObject(BBKEY_TARGET)))
	{
		Blackboard->SetValueAsObject(BBKEY_TARGET, Target);
		AActor::SetActorTickEnabled(true);
		FindTargetDelegate.Broadcast(true, Target);
	}
}

void APPAIController::ResetTarget()
{
	APawn* Target = Cast<APawn>(GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (Target)
	{
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);

		if (ASC)
		{
			FGameplayTagContainer Tag(PPTAG_CHARACTER_ISDEAD);
			if (ASC->HasAnyMatchingGameplayTags(Tag))
			{
				FindTargetDelegate.Broadcast(false, Target);
				GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
				AActor::SetActorTickEnabled(false);
			}
		}
	}
}
