// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/AbilityTask_GASPlayMontage.h"
#include "Project_P.h"
#include "AbilitySystemGlobals.h"

UAbilityTask_GASPlayMontage::UAbilityTask_GASPlayMontage()
{

}

UAbilityTask_GASPlayMontage* UAbilityTask_GASPlayMontage::CreatePlayMontage(UGameplayAbility* Abilitys, FName TaskName, UAnimMontage* Montage, float Rates, FName start, bool bStopAbilityEnds, float AnimTranslationScale, float StartSeconds, bool bAllowInterrupt)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rates);

	UAbilityTask_GASPlayMontage* MyObj = NewAbilityTask<UAbilityTask_GASPlayMontage>(Abilitys, TaskName);
	MyObj->MontageToPlay = Montage;
	MyObj->Rate = Rates;
	MyObj->StartSection = start;
	MyObj->AnimRootMotionTranslationScale = AnimTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopAbilityEnds;
	MyObj->bAllowInterruptAfterBlendOut = bAllowInterrupt;
	MyObj->StartTimeSeconds = StartSeconds;

	return MyObj;
}

void UAbilityTask_GASPlayMontage::Activate()
{
	PPNET_ATLOG(LogGAS, Log, TEXT("Begin"));

	Super::Activate();
}
