// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PPCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIsDeadDelegate);

/**
 * 
 */
UCLASS()
class PROJECT_P_API UPPCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPPCharacterAttributeSet();

	//#include "AbilitySystemComponent.h" Ãß°¡
	ATTRIBUTE_ACCESSORS(UPPCharacterAttributeSet, AttackRange);
	ATTRIBUTE_ACCESSORS(UPPCharacterAttributeSet, MaxAttackRange);
	ATTRIBUTE_ACCESSORS(UPPCharacterAttributeSet, AttackRadius);
	ATTRIBUTE_ACCESSORS(UPPCharacterAttributeSet, MaxAttackRadius);
	ATTRIBUTE_ACCESSORS(UPPCharacterAttributeSet, AttackRate);
	ATTRIBUTE_ACCESSORS(UPPCharacterAttributeSet, MaxAttackRate);
	ATTRIBUTE_ACCESSORS(UPPCharacterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UPPCharacterAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UPPCharacterAttributeSet, Damage);
	ATTRIBUTE_ACCESSORS(UPPCharacterAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS(UPPCharacterAttributeSet, MaxMana);

	mutable FIsDeadDelegate ActorIsDead;

	FORCEINLINE bool GetIsDead() const { return bIsDead; }
	FORCEINLINE void SetIsDead(bool NewIsDead) { bIsDead = NewIsDead; }

	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_AttackRange, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_MaxAttackRange, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_AttackRadius, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_MaxAttackRadius, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_AttackRate, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_MaxAttackRate, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Mana, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Mana;

	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MaxMana, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxMana;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	UPROPERTY(ReplicatedUsing = OnRep_IsDead)
	bool bIsDead = false;

protected:
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	void DamageEvent(const struct FGameplayEffectModCallbackData& Data);

protected:
	UFUNCTION()
	void OnRep_AttackRange(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxAttackRange(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_AttackRadius(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxAttackRadius(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_AttackRate(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxAttackRate(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_IsDead();
};
