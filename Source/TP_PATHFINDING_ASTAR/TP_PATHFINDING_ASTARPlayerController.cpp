// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_PATHFINDING_ASTARPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "TP_PATHFINDING_ASTARCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include <Kismet/KismetMathLibrary.h>
#include "Nodes.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ATP_PATHFINDING_ASTARPlayerController::ATP_PATHFINDING_ASTARPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void ATP_PATHFINDING_ASTARPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	FTransform GridTransform = UKismetMathLibrary::MakeTransform(FVector(1000, 1800, 92), FRotator(0, 0, 0));
	Grid = Cast<APathGrid>(GetWorld()->SpawnActor(APathGrid::StaticClass(), &GridTransform));

}

void ATP_PATHFINDING_ASTARPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ATP_PATHFINDING_ASTARPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &ATP_PATHFINDING_ASTARPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &ATP_PATHFINDING_ASTARPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &ATP_PATHFINDING_ASTARPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &ATP_PATHFINDING_ASTARPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &ATP_PATHFINDING_ASTARPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &ATP_PATHFINDING_ASTARPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &ATP_PATHFINDING_ASTARPlayerController::OnTouchReleased);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATP_PATHFINDING_ASTARPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void ATP_PATHFINDING_ASTARPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_WorldDynamic, true, Hit);
	}
	else
	{
		//TEnumAsByte<ETraceTypeQuery> TraceChannel;
		//GetHitResultUnderCursorByChannel(TraceChannel,true,Hit);
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	if (Hit.GetActor()->IsA<ANodes>())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Screen Message"));
	}
	
	// Move towards mouse pointer or touch
	//APawn* ControlledPawn = GetPawn();
	//if (ControlledPawn != nullptr)
	//{
	//	FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
	//	ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	//}
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, Hit.GetActor()->GetName());
}

void ATP_PATHFINDING_ASTARPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		//UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
	Grid->LauchSimulation();
}

// Triggered every frame when the input is held down
void ATP_PATHFINDING_ASTARPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void ATP_PATHFINDING_ASTARPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}
