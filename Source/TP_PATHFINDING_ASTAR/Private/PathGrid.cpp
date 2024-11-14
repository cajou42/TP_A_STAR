// Fill out your copyright notice in the Description page of Project Settings.


#include "PathGrid.h"
#include "Obstacle.h"
#include <Kismet/KismetMathLibrary.h>
#include "TimerManager.h"

// Sets default values
APathGrid::APathGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	for (int i = 0; i < Height; i++)
	{
		BlockX.Add(0);
	}

	for (int j = 0; j < Width; j++)
	{
		BlockY.Add(0);
	}
}

// Called when the game starts or when spawned
void APathGrid::BeginPlay()
{
	Super::BeginPlay();

	FVector CurrentPoint = StartingGridPoint;
	BlockX[3] = 2;
	BlockX[4] = 1;
	BlockY[3] = 2;
	BlockY[2] = 1;
	BlockX[1] = 1;
	BlockY[1] = 1;
	
	// Init the grid
	for (double i = 0; i < Height; i++) 
	{
		for (double j = 0; j < Width; j++) 
		{

			FTransform NodeTransform = UKismetMathLibrary::MakeTransform(CurrentPoint, FRotator(0, 0, 0));
			ANodes* TempNodes = Cast<ANodes>(GetWorld()->SpawnActor(ANodes::StaticClass(), &NodeTransform));
			TempNodes->Index = FVector2D(i, j);
			NodesList.Add(TempNodes);

			// Spwaning StartingPoint
			if (i == StartingPoint.X && j == StartingPoint.Y)
			{
				DrawDebugBox(GetWorld(), CurrentPoint, FVector(50, 50, 50), GetActorQuat(), FColor::Blue, true, 0.01, 0, 5);
				StartingPoint = CurrentPoint;
				StartingNode = TempNodes;
			}
			// Spwaning FinishPoint
			else if (i == FinishPoint.X && j == FinishPoint.Y)
			{
				DrawDebugBox(GetWorld(), CurrentPoint, FVector(50, 50, 50), GetActorQuat(), FColor::Cyan, true, 0.01, 0, 5);
				FinishPoint = CurrentPoint;
				FinishNode = TempNodes;
			}
			// Spwaning Obstacle
			else if (BlockX[i] && BlockY[j])
			{
				DrawDebugBox(GetWorld(), CurrentPoint, FVector(50, 50, 50), GetActorQuat(), FColor::Black, true, 0.01, 0, 5);
				BlockX[i]--;
				BlockY[j]--;
				TempNodes->IsObstacle = true;
			}
			// Spwaning normal case
			else
			{
				DrawDebugBox(GetWorld(), CurrentPoint, FVector(50, 50, 50), GetActorQuat(), FColor::Red, true, 0.01, 0, 5);
			}

			CurrentPoint.Y += 110;
		}
		CurrentPoint.Y = StartingGridPoint.Y;
		CurrentPoint.X += 110;
	}
}

// Called every frame
void APathGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APathGrid::TimerElapsed()
{
	UE_LOG(LogTemp, Warning, TEXT("Délai terminé !"));
}

// Brief : Function that give the lowest weight node in function of his distance between the departure and the arrive
// Return : The lowest weight node
ANodes* APathGrid::GetLowerWeightNodes()
{
	int lowerWeight = 100;
	ANodes* LowerNode = nullptr;

	for (int i = 0; i < AdjacentNodes.Num(); i++)
	{
		AdjacentNodes[i]->Weight = GetDistance(AdjacentNodes[i], FinishNode) + GetDistance(AdjacentNodes[i], StartingNode);

		if (AdjacentNodes[i]->Weight < lowerWeight)
		{
			lowerWeight = AdjacentNodes[i]->Weight;
			LowerNode = AdjacentNodes[i];
		}
	}

	return LowerNode;
}

// Brief : Function that give the distance between two nodes
// Parameters :
//		starting : Node from which we start
//		finish   : Node to reach
// Return : the distance between the two node as the number of nodes traveled
int APathGrid::GetDistance(ANodes* starting, ANodes* finish)
{
	int x = starting->Index.X;
	int y = starting->Index.Y;
	int Distance = 0;
	while (true)
	{
		if (x == finish->Index.X && y == finish->Index.Y)
			break;

		if (x < finish->Index.X && y < finish->Index.Y)
		{
			x++;
			y++;
			Distance += 14;
			continue;
		}
		else if (x > finish->Index.X && y > finish->Index.Y)
		{
			x--;
			y--;
			Distance += 14;
			continue;
		}
		else if (x < finish->Index.X && y > finish->Index.Y)
		{
			x++;
			y--;
			Distance += 14;
			continue;
		}
		else if (x > finish->Index.X && y < finish->Index.Y)
		{
			x--;
			y++;
			Distance += 14;
			continue;
		}

		if (x < finish->Index.X)
		{
			x++;
			Distance += 10;
		}
		else if (x > finish->Index.X)
		{
			x--;
			Distance += 10;
		}

		if (y < finish->Index.Y)
		{
			y++;
			Distance += 10;
		}
		else if (y > finish->Index.Y)
		{
			y--;
			Distance += 10;
		}
	}
	return Distance;
}

// Brief : Function that lauch the A* Pathfinding
void APathGrid::LauchSimulation()
{
	ANodes* CurentNode = StartingNode;
	AdjacentNodes.Add(StartingNode);
	//loop
	while (CurentNode->Index != FinishNode->Index) 
	{
		GetAdjacentNodes(CurentNode->Index);
		CurentNode = GetLowerWeightNodes();
		DrawDebugBox(GetWorld(), CurentNode->GetActorLocation(), FVector(50, 50, 50), GetActorQuat(), FColor::Green, true, 0.01, 0, 5);
		CurentNode->IsObstacle = true;
		AdjacentNodes.Remove(CurentNode);
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &APathGrid::TimerElapsed, 2.0f, false);
	}
	
}

// Brief : Function that get all nodes around a given node
// Parameter :
//		Position : Position of the node
void APathGrid::GetAdjacentNodes(FVector2D Position)
{
	// Up
	if (Position.X + 1 < Height && !NodesList[(Position.X + 1) * Height + Position.Y]->IsObstacle)
		AdjacentNodes.Add(NodesList[(Position.X + 1) * Height + Position.Y]);

	// Down
	if (Position.X - 1 >= 0 && !NodesList[(Position.X - 1) * Height + Position.Y]->IsObstacle)
		AdjacentNodes.Add(NodesList[(Position.X - 1) * Height + Position.Y]);

	// Right
	if (Position.Y + 1 > Width && !NodesList[(Position.X) * Height + (Position.Y + 1)]->IsObstacle)
		AdjacentNodes.Add(NodesList[Position.X * Height + (Position.Y + 1)]);

	// Left
	if (Position.Y - 1 >= 0 && !NodesList[(Position.X) * Height + (Position.Y - 1)]->IsObstacle)
		AdjacentNodes.Add(NodesList[Position.X * Height + (Position.Y - 1)]);

	// Down Left
	if (Position.X - 1 >= 0 &&  Position.Y - 1 >= 0 && !NodesList[(Position.X - 1) * Height + (Position.Y - 1)]->IsObstacle)
		AdjacentNodes.Add(NodesList[(Position.X - 1) * Height + (Position.Y - 1)]);

	// Up Left
	if (Position.X + 1 < Height && Position.Y - 1 >= 0 && !NodesList[(Position.X + 1) * Height + (Position.Y - 1)]->IsObstacle)
		AdjacentNodes.Add(NodesList[(Position.X + 1) * Height + (Position.Y - 1)]);

	// Down Rigth
	if (Position.X - 1 >= 0 && Position.Y + 1 < Width && !NodesList[(Position.X - 1) * Height + (Position.Y + 1)]->IsObstacle)
		AdjacentNodes.Add(NodesList[(Position.X - 1) * Height + (Position.Y + 1)]);

	// Up Rigth
	if (Position.X + 1 < Height && Position.Y + 1 < Width && !NodesList[(Position.X + 1) * Height + (Position.Y + 1)]->IsObstacle)
		AdjacentNodes.Add(NodesList[(Position.X + 1) * Height + (Position.Y + 1)]);
}


