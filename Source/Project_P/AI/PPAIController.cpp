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

APPAIController::APPAIController()
{
	//#include "BehaviorTree/BlackboardData.h"�߰�
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardRef(TEXT("/Script/AIModule.BlackboardData'/Game/Project_P/AI/BB_PPCharacter.BB_PPCharacter'"));
	if (BlackboardRef.Object)
	{
		BBAsset = BlackboardRef.Object;
	}

	//#include "BehaviorTree/BehaviorTree.h"�߰�
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Project_P/AI/BT_PPCharacter.BT_PPCharacter'"));
	if(BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
	
// AI Perception ����
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	SetPerceptionComponent(*AIPerceptionComp);

	// Sight Config
	SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfig_Sight"));

	AIPerceptionComp->ConfigureSense(*SenseConfig_Sight);
	AIPerceptionComp->SetDominantSense(SenseConfig_Sight->GetSenseImplementation());

	AIPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &APPAIController::PerceptionUpdated);

	// Hearing Config
	SenseConfig_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("SenseConfig_Hearing"));

	// Damage Config
	SenseConfig_Damage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("SenseConfig_Damage"));
}

void APPAIController::RunAI()
{
	//�θ� Ŭ������ UBlackboardComponent�� ���������
	UBlackboardComponent* BBComponent = Blackboard.Get();
	if (UseBlackboard(BBAsset, BBComponent))
	{
		//������ �����Ϳ� ������
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void APPAIController::StopAI()
{
	//UBehaviorTreeComponent�� BrainComponent�� �ڽ� Ŭ����
	//�θ� Ŭ������ BrainComponent�� ���� �Ǿ�����
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
			//Ž���ϸ� ����(age)���� Ž���� ��������
			SenseConfig_Sight->SetMaxAge(GruntAttributeSet->GetAISenseAge());
			//���������� ������ ��ü�� ��ġ Ž�� ���� - �þ� ������ ��� ���¿����� ��ü�� ���� �Ÿ� �̳����� ������ �� �ִ�.
			SenseConfig_Sight->AutoSuccessRangeFromLastSeenLocation = -1.f;

			SenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
			SenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = true;
			SenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = true;
		}
	}
}

void APPAIController::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		if (Actor->ActorHasTag("Player"))
		{
			UE_LOG(LogTemp, Log, TEXT("PercetionedActor : %s"), *Actor->GetName());
		}
	}
}
