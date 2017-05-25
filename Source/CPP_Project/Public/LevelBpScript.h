// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/LevelScriptActor.h"
#include "LevelBpScript.generated.h"

/**
 * 
 */
UCLASS()
class CPP_PROJECT_API ALevelBpScript : public ALevelScriptActor
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	ULightComponent* Light;
	
	
};
