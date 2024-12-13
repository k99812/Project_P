// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/PPCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Project_P.h"
#include "Tag/PPGameplayTag.h"

UPPCharacterAttributeSet::UPPCharacterAttributeSet() : 
	AttackRange(100.0f), MaxAttackRange(300.0f),
	AttackRadius(50.0f), MaxAttackRadius(150.0f),
	AttackRate(30.0f), MaxAttackRate(100.0f),
	MaxHealth(100.0f), Damage(0), Mana(100.0f), MaxMana(100.0f)
{
	InitHealth(GetMaxHealth());
}

bool UPPCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	return true;
}

void UPPCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinHealth = 0.0f;

	//#include "GameplayEffectExtension.h" 추가
	//Data에 실행되는 GE스펙, 타겟 ASC, EvaluatedData에 변경되는 어트리뷰트 및 연산자 등이 들어있음
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		PPGAS_LOG(LogGAS, Warning, TEXT("Direct Healt Accesss : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth(), MinHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		PPGAS_LOG(LogGAS, Log, TEXT("Direct Healt Accesss : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinHealth, GetMaxHealth()));
		SetDamage(0.0f);
	}

	if (GetHealth() <= 0.0f && !bIsDead)
	{
		Data.Target.AddLooseGameplayTag(PPTAG_CHARACTER_ISDEAD);
		//Data.Target.AddReplicatedLooseGameplayTag(PPTAG_CHARACTER_ISDEAD);

		ActorIsDead.Broadcast();
	}

	bIsDead = GetHealth() <= 0.0f;
}

void UPPCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}
