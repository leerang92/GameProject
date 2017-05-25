// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "CPP_Project.h"
#include "CPP_ProjectGameMode.h"
#include "CPP_ProjectCharacter.h"
#include "ProjectHUD.h"

ACPP_ProjectGameMode::ACPP_ProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	static ConstructorHelpers::FClassFinder<AProjectHUD> ProjectHUDClass(TEXT("Class'/Script/CPP_Project.ProjectHUD'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		HUDClass = ProjectHUDClass.Class;
	}
}
