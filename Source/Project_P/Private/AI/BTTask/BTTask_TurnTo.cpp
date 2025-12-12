// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_TurnTo.h"
#include "AI/PPAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/PPAI.h"

UBTTask_TurnTo::UBTTask_TurnTo()
{
	bNotifyTick = true;
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Retsult = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!IsValid(ControllingPawn))
	{
		return EBTNodeResult::Type::Failed;
	}

	FVector TargetPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
	FVector NowPos = ControllingPawn->GetActorLocation();
	FVector LookVector = TargetPos - NowPos;
	FRotator LookRotation = FRotationMatrix::MakeFromX(LookVector).Rotator(); 
	OwnerComp.GetBlackboardComponent()->SetValueAsRotator(BBKEY_TARGETROT, LookRotation);

	Retsult = EBTNodeResult::Type::InProgress;

	return Retsult;
}

void UBTTask_TurnTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!IsValid(ControllingPawn))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FRotator CurrentRot = ControllingPawn->GetActorRotation();
	FRotator TargetRot = OwnerComp.GetBlackboardComponent()->GetValueAsRotator(BBKEY_TARGETROT);
	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaSeconds, TurnSpeed);
	OwnerComp.GetAIOwner()->SetControlRotation(NewRot);

	float YawDiff = FMath::Abs(FRotator::NormalizeAxis(NewRot.Yaw - TargetRot.Yaw));
	if (YawDiff < AcceptableRedius)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
