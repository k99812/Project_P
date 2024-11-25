// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PPAIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PPAI.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Sight.h"
#include "Tag/PPGameplayTag.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "Data/PPGruntAIData.h"

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

	static ConstructorHelpers::FObjectFinder<UPPGruntAIData> AIDataRef(TEXT("/Script/Project_P.PPGruntAIData'/Game/Project_P/Data/GruntAIData.GruntAIData'"));
	if (AIDataRef.Object)
	{
		GruntAIData = AIDataRef.Object;
	}
	
// AI Perception ����
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	SetPerceptionComponent(*AIPerceptionComp);

	// Sight Config
	SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfig_Sight"));

	SenseConfig_Sight->SightRadius = GruntAIData->SightRadius;
	SenseConfig_Sight->LoseSightRadius = GruntAIData->LoseSightRadius;
	SenseConfig_Sight->PeripheralVisionAngleDegrees = GruntAIData->AIVisionAngleDeg;
	//Ž���ϸ� ����(age)���� Ž���� ��������
	SenseConfig_Sight->SetMaxAge(GruntAIData->AISenseAge);
	//���������� ������ ��ü�� ��ġ Ž�� ���� - �þ� ������ ��� ���¿����� ��ü�� ���� �Ÿ� �̳����� ������ �� �ִ�.
	SenseConfig_Sight->AutoSuccessRangeFromLastSeenLocation = GruntAIData->AutoSuccessRange;

	SenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	SenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = true;
	SenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComp->ConfigureSense(*SenseConfig_Sight);
	AIPerceptionComp->SetDominantSense(SenseConfig_Sight->GetSenseImplementation());

	// Hearing Config
	SenseConfig_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("SenseConfig_Hearing"));

	// Damage Config
	SenseConfig_Damage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("SenseConfig_Damage"));

	AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &APPAIController::ActorPerceptionUpdated);
	AIPerceptionComp->OnTargetPerceptionForgotten.AddDynamic(this, &APPAIController::ActorPerceptionForgetUpdated);
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
}

void APPAIController::ActorPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	IGameplayTagAssetInterface* TagActor = Cast<IGameplayTagAssetInterface>(Actor);

	if (TagActor && Stimulus.WasSuccessfullySensed())
	{
		FGameplayTagContainer TagContainer(PPTAG_CHARACTER_PLAYER);

		if (TagActor->HasAnyMatchingGameplayTags(TagContainer))
		{
			UE_LOG(LogTemp, Log, TEXT("ActorPerceptionUpdated : %s"), *Actor->GetName());

			TSubclassOf<UAISense> SensedStimulsClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus);

			if (SensedStimulsClass == UAISense_Sight::StaticClass())
			{
				UE_LOG(LogTemp, Log, TEXT("AI Sense Sight Perception Updated "));
			}
		}
	}
}

void APPAIController::ActorPerceptionForgetUpdated(AActor* Actor)
{
	IGameplayTagAssetInterface* TagActor = Cast<IGameplayTagAssetInterface>(Actor);

	if (TagActor)
	{
		FGameplayTagContainer TagContainer(PPTAG_CHARACTER_PLAYER);

		if (TagActor->HasAnyMatchingGameplayTags(TagContainer))
		{
			UE_LOG(LogTemp, Log, TEXT("ActorPerceptionForgetUpdated : %s"), *Actor->GetName());
		}
	}
}
