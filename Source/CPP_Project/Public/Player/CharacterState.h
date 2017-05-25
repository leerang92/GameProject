// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "CharacterState.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	PS_Idle			UMETA(DisplayeName = "Idle"),
	PS_Walk			UMETA(DisplayeName = "Walk"),
	PS_Run			UMETA(DisplayeName = "Run"),
	PS_Attack		UMETA(DisplayeName = "Attack"),
	PS_Die			UMETA(DisplayeName = "Die"),
	PS_Fire			UMETA(DisplayeName = "Fire"),
	PS_Reload		UMETA(DisplayeName = "Reload"),
};
/**
 * 
 */
//UCLASS()
//class CPP_PROJECT_API UCharacterState : public UObject
//{
//	GENERATED_BODY()
//	
//	
//	
//	
//};
