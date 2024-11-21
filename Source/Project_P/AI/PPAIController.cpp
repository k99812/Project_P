// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PPAIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PPAI.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PPGruntAttributeSet.h"
#include "Tag/PPGameplayTag.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"

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
	
// AI Perception 설정
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	SetPerceptionComponent(*AIPerceptionComp);

	// Sight Config
	SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfig_Sight"));

	SenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	SenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = true;
	SenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComp->ConfigureSense(*SenseConfig_Sight);
	AIPerceptionComp->SetDominantSense(SenseConfig_Sight->GetSenseImplementation());

	// Hearing Config
	SenseConfig_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("SenseConfig_Hearing"));

	// Damage Config
	SenseConfig_Damage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("SenseConfig_Damage"));

	AIPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &APPAIController::PerceptionUpdated);
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

	ControlledPawn = InPawn;

	RunAI();
}

void APPAIController::BeginPlay()
{
	Super::BeginPlay();

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControlledPawn);
	if (ASC)
	{
		const UPPGruntAttributeSet* GruntAttributeSet = ASC->GetSet<UPPGruntAttributeSet>();
		if (GruntAttributeSet)
		{
			SenseConfig_Sight->SightRadius = GruntAttributeSet->GetAIDetectRadius();
			SenseConfig_Sight->LoseSightRadius = GruntAttributeSet->GetAILoseRadius();
			SenseConfig_Sight->PeripheralVisionAngleDegrees = GruntAttributeSet->GetAIVisionAngleDeg();
			//탐지하면 몇초(age)동안 탐지를 유지할지
			SenseConfig_Sight->SetMaxAge(GruntAttributeSet->GetAISenseAge());
			//마지막으로 감지된 객체의 위치 탐지 성공 - 시야 범위를 벗어난 상태에서도 객체를 일정 거리 이내에서 추적할 수 있다.
			SenseConfig_Sight->AutoSuccessRangeFromLastSeenLocation = -1.f;
		}
	}
}

void APPAIController::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	UE_LOG(LogTemp, Log, TEXT("PerceptionUpdated"));

	for (AActor* Actor : UpdatedActors)
	{
		UE_LOG(LogTemp, Log, TEXT("PerceptionUpdated : %s"), *Actor->GetName());

		/*
		IGameplayTagAssetInterface* TagActor = Cast<IGameplayTagAssetInterface>(Actor);

		if (TagActor)
		{
			UE_LOG(LogTemp, Log, TEXT("TagActor : %s"), *Actor->GetName());
			FGameplayTagContainer TagContainer(PPTAG_CHARACTER_PLAYER);

			if (TagActor->HasAnyMatchingGameplayTags(TagContainer))
			{
				UE_LOG(LogTemp, Log, TEXT("TagActor Matching Player : %s"), *Actor->GetName());
			}
		}
		*/
	}
}
