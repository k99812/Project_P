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
	//AnimInstance가 생성될때 한번만 호출
	virtual void NativeInitializeAnimation() override;

	//Tick마다 호출
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

	//움직이는지 검사하기 위한 변수 GroundSpeed < MovingThreshould
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsSprint : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	//점프하는지 검삭하기 위한 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;

	//Z축
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float Yaw;

	//Y축
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float Pitch;

	//X축
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float Roll;

	UPROPERTY(BlueprintReadOnly, Category = ASC)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	FGameplayTagContainer SprintTagContainer;

protected:
	void SaveLastDirection();
};
