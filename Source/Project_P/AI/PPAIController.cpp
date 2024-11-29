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
#include "Data/PPGruntAIData.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagContainer.h"
#include "Tag/PPGameplayTag.h"

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

// Tick
	PrimaryActorTick.bCanEverTick = true;
	//AActor::SetActorTickEnabled() �Լ��� ����
	PrimaryActorTick.bStartWithTickEnabled = false;
	
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

void APPAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ResetTarget();
}

void APPAIController::ActorPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	APawn* Pawn_ = Cast<APawn>(Actor);

	if (Pawn_ && Pawn_->GetController()->IsPlayerController())
	{
		TSubclassOf<UAISense> SensedStimulsClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus);

		if (SensedStimulsClass == UAISense_Sight::StaticClass())
		{
			PerceptionSensedSight(Pawn_);
		}
	}
}

void APPAIController::ActorPerceptionForgetUpdated(AActor* Actor)
{
	APawn* Pawn_ = Cast<APawn>(Actor);

	if (Pawn_ && Pawn_->GetController()->IsPlayerController())
	{
		UE_LOG(LogTemp, Log, TEXT("ActorPerceptionForgetUpdated : %s"), *Actor->GetName());

		APawn* Target = Cast<APawn>(GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
		if (Pawn_ == Target)
		{
			GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
			AActor::SetActorTickEnabled(false);
		}
	}
}

void APPAIController::PerceptionSensedSight(APawn* Pawn_)
{
	UE_LOG(LogTemp, Log, TEXT("ActorPerceptionUpdated : %s"), *Pawn_->GetName());

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Pawn_);
	if (ASC)
	{
		GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn_);
		AActor::SetActorTickEnabled(true);
	}
}

void APPAIController::PerceptionSensedHearing(APawn* Pawn_)
{
}

void APPAIController::PerceptionSensedDamage(APawn* Pawn_)
{
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
				GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
				AActor::SetActorTickEnabled(false);
			}
		}
	}
}
