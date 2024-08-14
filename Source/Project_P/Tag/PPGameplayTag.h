
#pragma once

#include "GameplayTagContainer.h"

#define PPTAG_ABILITY_JUMP FGameplayTag::RequestGameplayTag(FName("Ability.Jump"))
#define PPTAG_ABILITY_ATTACK FGameplayTag::RequestGameplayTag(FName("Ability.Attack"))
#define PPTAG_ABILITY_SPRINT FGameplayTag::RequestGameplayTag(FName("Ability.Sprint"))

#define PPTAG_CHARACTER_ISATTACKING FGameplayTag::RequestGameplayTag(FName("Character.State.IsAttacking"))
#define PPTAG_CHARACTER_ISJUMPING FGameplayTag::RequestGameplayTag(FName("Character.State.IsJumping"))
#define PPTAG_CHARACTER_ISSPRINT FGameplayTag::RequestGameplayTag(FName("Character.State.IsSprint"))
#define PPTAG_CHARACTER_ISWALKING FGameplayTag::RequestGameplayTag(FName("Character.State.IsWalking"))
#define PPTAG_CHARACTER_ISCC FGameplayTag::RequestGameplayTag(FName("Character.State.IsCC"))