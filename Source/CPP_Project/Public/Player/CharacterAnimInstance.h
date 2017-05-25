// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "CPP_ProjectCharacter.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CPP_PROJECT_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	
public:

	UFUNCTION()
	void AnimNotify_ReloadEnd(UAnimNotify * Notify);
};
