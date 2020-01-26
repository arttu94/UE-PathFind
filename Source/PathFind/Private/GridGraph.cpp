// Fill out your copyright notice in the Description page of Project Settings.

#include "GridGraph.h"
#include "Engine/World.h"
#include "PathNode.h"
#include "Containers/Queue.h"
#include "GraphSearch.h"

#include "Engine.h"

// Sets default values
AGridGraph::AGridGraph()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bAllowDiagonalMovement = false;
}

// Called when the game starts or when spawned
void AGridGraph::BeginPlay()
{
	Super::BeginPlay();

	for (size_t x = 0; x < SquareSize; x++)
	{
		Grid.Add(TArray<APathNode*>());
		for (size_t y = 0; y < SquareSize; y++)
		{
			if (NodeBlueprintClass)
			{
				FActorSpawnParameters SpawnParams;

				Grid[x].Add(GetWorld()->SpawnActor<APathNode>(NodeBlueprintClass, SpawnParams));

				/*if (x == 0 || x == 19 || y == 0 || y == 19)
					Grid[x][y]->SetNodeType(EPathNodeType::PN_Closed);
				else
					Grid[x][y]->SetNodeType(EPathNodeType::PN_Open);*/

				Grid[x][y]->SetNodeType(EPathNodeType::PN_Open);

				Grid[x][y]->SetActorLocation(FVector(x * 100, y * -100, 0.f));

				Grid[x][y]->UpdateNodeType();
				Grid[x][y]->SetNodeWeight(1);
				Grid[x][y]->Graph = this;
				Grid[x][y]->SetGraphLocation(FGraphLocation(x, y));
			}
		}
	}

	for (size_t x = 0; x < SquareSize; x++)
	{
		for (size_t y = 0; y < SquareSize; y++)
		{
			//Grid[x][y]->Neighbours = TArray<APathNode*>();

			if (Grid[x][y]->GetNodeType() != EPathNodeType::PN_Closed)
			{
				if (y < SquareSize - 1)
				{
					Grid[x][y]->AddNeighbourNode(Grid[x][y + 1]);
					if(bAllowDiagonalMovement && x < SquareSize - 1)
						Grid[x][y]->AddNeighbourNode(Grid[x + 1][y + 1]);
				}
				if (x < SquareSize - 1)
				{
					Grid[x][y]->AddNeighbourNode(Grid[x + 1][y]);
					if(bAllowDiagonalMovement && y > 0)
						Grid[x][y]->AddNeighbourNode(Grid[x + 1][y - 1]);
				}
				if (y > 0)
				{
					Grid[x][y]->AddNeighbourNode(Grid[x][y - 1]);
					if(bAllowDiagonalMovement && x > 0)
						Grid[x][y]->AddNeighbourNode(Grid[x - 1][y - 1]);
				}
				if (x > 0)
				{
					Grid[x][y]->AddNeighbourNode(Grid[x - 1][y]);
					if(bAllowDiagonalMovement && y < SquareSize - 1)
						Grid[x][y]->AddNeighbourNode(Grid[x - 1][y + 1]);
				}
			}
		}
	}

	Start = Grid[0][3];
	Finish = Grid[11][13];

	Finish->SetNodeType(EPathNodeType::PN_Finish);
	Finish->UpdateNodeType();
	Start->SetNodeType(EPathNodeType::PN_Start);
	Start->UpdateNodeType();

	//UGraphSearch::BreadthFirstSearch(Start); 
}

void AGridGraph::ClearGraphNodes()
{
	for (auto nodeArray : Grid)
	{
		for (auto node : nodeArray)
		{
			node->SetCameFrom(nullptr);
			//node->SetCostSoFar(0);
		}
	}
}

// Called every frame
//void AGridGraph::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

