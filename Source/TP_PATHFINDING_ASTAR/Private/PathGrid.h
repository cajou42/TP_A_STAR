// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/BoxComponent.h>
#include "Nodes.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathGrid.generated.h"

UCLASS()
class APathGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathGrid();
	FVector StartingGridPoint = FVector(1000, 1800, 92);
	double Height = 5;
	double Width = 5;
	TArray<double> BlockX;
	TArray<double> BlockY;

	FVector StartingPoint = FVector(4, 4, 0);
	FVector FinishPoint = FVector(2, 0, 0);

	TArray<ANodes*> NodesList;
	ANodes* StartingNode;
	ANodes* FinishNode;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void LauchSimulation();

private :
	TArray<ANodes*> AdjacentNodes;
	FTimerHandle UnusedHandle;

	ANodes* GetLowerWeightNodes();
	int GetDistance(ANodes* starting, ANodes* finish);
	void GetAdjacentNodes(FVector2D Position);
	void TimerElapsed();
};