// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Project.h"
#include "CharacterAnimInstance.h"

void UCharacterAnimInstance::AnimNotify_ReloadEnd(UAnimNotify * Notify)
{
	ACPP_ProjectCharacter* Character = Cast<ACPP_ProjectCharacter>(TryGetPawnOwner());

	if (Character)
	{
		Character->BasicWeapon->StartReload();
	}
}


