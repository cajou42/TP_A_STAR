// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_PATHFINDING_ASTARGameMode.h"
#include "TP_PATHFINDING_ASTARPlayerController.h"
#include "TP_PATHFINDING_ASTARCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATP_PATHFINDING_ASTARGameMode::ATP_PATHFINDING_ASTARGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATP_PATHFINDING_ASTARPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}