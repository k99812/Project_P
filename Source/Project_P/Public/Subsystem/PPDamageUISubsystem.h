// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PPDamageUISubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API UPPDamageUISubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UPPDamageUISubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

public:
	void ShowDamageUI(TSubclassOf<class UPPFloatingTextUserWidget> WidgetClass, float Damage, const FVector& WorldLocation, APlayerController* OwnerPC);

	void ReturnToPool(class UPPFloatingTextUserWidget* Widget);

private:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<class UPPFloatingTextUserWidget>> DamageUIPool;
};
