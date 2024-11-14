// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/BoxComponent.h>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Nodes.generated.h"

UCLASS()
class TP_PATHFINDING_ASTAR_API ANodes : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANodes();
	void SetWeight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	UBoxComponent* BoxCollider;

	int StartingDistance;
	int DestinationDistance;
	int Weight;

	FVector2D Index;

	bool IsObstacle;
	bool IsUsed;

};
