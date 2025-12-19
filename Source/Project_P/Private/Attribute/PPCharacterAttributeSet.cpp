// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/PPCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Project_P.h"
#include "Tag/PPGameplayTag.h"
#include "Net/UnrealNetwork.h"
#include "Character/PPGASCharacterPlayer.h"

UPPCharacterAttributeSet::UPPCharacterAttributeSet() : 
	AttackRange(100.0f), MaxAttackRange(300.0f),
	AttackRadius(50.0f), MaxAttackRadius(150.0f),
	AttackRate(30.0f), MaxAttackRate(100.0f),
	MaxHealth(100.0f), Damage(0), Mana(100.0f), MaxMana(100.0f)
{
	InitHealth(GetMaxHealth());
}

void UPPCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPPCharacterAttributeSet, AttackRange, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPPCharacterAttributeSet, MaxAttackRange, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPPCharacterAttributeSet, AttackRadius, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPPCharacterAttributeSet, MaxAttackRadius, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPPCharacterAttributeSet, AttackRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPPCharacterAttributeSet, MaxAttackRate, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPPCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPPCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPPCharacterAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPPCharacterAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME(UPPCharacterAttributeSet, bIsDead);
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

		if (AActor* Target = Data.Target.GetAvatarActor())
		{
			Target->ForceNetUpdate();
		}
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		PPGAS_LOG(LogGAS, Log, TEXT("Direct Healt Accesss : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinHealth, GetMaxHealth()));
		SetDamage(0.0f);

		if (AActor* Target = Data.Target.GetAvatarActor())
		{
			Target->ForceNetUpdate();
		}
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

void UPPCharacterAttributeSet::OnRep_IsDead()
{
	PPNET_ATTLOG(LogGAS, Log, TEXT("Begin"));

	if (bIsDead)
	{
		ActorIsDead.Broadcast();
	}

	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	if (ASC)
	{
		
		if (bIsDead)
		{
			ASC->AddLooseGameplayTag(PPTAG_CHARACTER_ISDEAD);
		}
		else
		{
			// 만약 부활 기능이 있다면 태그 제거도 필요
			ASC->RemoveLooseGameplayTag(PPTAG_CHARACTER_ISDEAD);
		}

		APPGASCharacterPlayer* Player = Cast<APPGASCharacterPlayer>(ASC->GetAvatarActor());
		if (Player)
		{
			if (bIsDead)
			{
				Player->SetDead();
			}
			else
			{
				Player->SetAlive();
			}
		}
	}
}

void UPPCharacterAttributeSet::OnRep_AttackRange(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPPCharacterAttributeSet, AttackRange, OldValue);
}

void UPPCharacterAttributeSet::OnRep_MaxAttackRange(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPPCharacterAttributeSet, MaxAttackRange, OldValue);
}

void UPPCharacterAttributeSet::OnRep_AttackRadius(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPPCharacterAttributeSet, AttackRadius, OldValue);
}

void UPPCharacterAttributeSet::OnRep_MaxAttackRadius(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPPCharacterAttributeSet, MaxAttackRadius, OldValue);
}

void UPPCharacterAttributeSet::OnRep_AttackRate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPPCharacterAttributeSet, AttackRate, OldValue);
}

void UPPCharacterAttributeSet::OnRep_MaxAttackRate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPPCharacterAttributeSet, MaxAttackRate, OldValue);
}

void UPPCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPPCharacterAttributeSet, Health, OldValue);
}

void UPPCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPPCharacterAttributeSet, MaxHealth, OldValue);
}

void UPPCharacterAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPPCharacterAttributeSet, Mana, OldValue);
}

void UPPCharacterAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPPCharacterAttributeSet, MaxMana, OldValue);
}