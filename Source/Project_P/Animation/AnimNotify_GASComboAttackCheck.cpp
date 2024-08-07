// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_GASComboAttackCheck.h"
#include "Character/PPGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "GA/PPGA_Attack.h"

UAnimNotify_GASComboAttackCheck::UAnimNotify_GASComboAttackCheck()
{
}

FString UAnimNotify_GASComboAttackCheck::GetNotifyName_Implementation() const
{
	return TEXT("GASComboAttackCheck");
}

void UAnimNotify_GASComboAttackCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		APPGASCharacterPlayer* OwnerPlayer = Cast<APPGASCharacterPlayer>(MeshComp->GetOwner());
		/*if (OwnerPlayer)
		{
			UAbilitySystemComponent* ASC = OwnerPlayer->GetAbilitySystemComponent();
			FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID((int32)EInputAbility::LeftAttack);
			if (Spec)
			{
				UPPGA_Attack* AttackGA = CastChecked<UPPGA_Attack>(Spec->Ability);
				AttackGA->CheckComboInput();
			}
		}*/
	}
}
