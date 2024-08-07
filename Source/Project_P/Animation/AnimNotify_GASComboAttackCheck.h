// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_GASComboAttackCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API UAnimNotify_GASComboAttackCheck : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_GASComboAttackCheck();

protected:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
