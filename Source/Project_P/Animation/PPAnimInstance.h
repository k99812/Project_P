// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "PPAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API UPPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPPAnimInstance();

protected:
	//AnimInstance�� �����ɶ� �ѹ��� ȣ��
	virtual void NativeInitializeAnimation() override;

	//Tick���� ȣ��
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeBeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class APPCharacterBase> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float LastDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	//�����̴��� �˻��ϱ� ���� ���� GroundSpeed < MovingThreshould
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsSprint : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	//�����ϴ��� �˻��ϱ� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;

	//Z��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float Yaw;

	//Y��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float Pitch;

	//X��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float Roll;

	UPROPERTY(BlueprintReadOnly, Category = ASC)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	FGameplayTagContainer SprintTagContainer;

protected:
	void SaveLastDirection();
};
