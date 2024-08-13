
#pragma once

#include "GameplayTagContainer.h"

#define PPTAG_CHARACTER_ISATTACKING FGameplayTag::RequestGameplayTag(FName("Character.State.IsAttacking"))
#define PPTAG_CHARACTER_ISJUMPING FGameplayTag::RequestGameplayTag(FName("Character.State.IsJumping"))
#define PPTAG_CHARACTER_ISSPRINT FGameplayTag::RequestGameplayTag(FName("Character.State.IsSprint"))
#define PPTAG_CHARACTER_ISWALKING FGameplayTag::RequestGameplayTag(FName("Character.State.IsWalking"))