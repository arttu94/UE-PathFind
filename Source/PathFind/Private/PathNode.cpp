// Fill out your copyright notice in the Description page of Project Settings.


#include "PathNode.h"
#include "Components/StaticMeshComponent.h"
#include "GridGraph.h"

#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"

// Sets default values
APathNode::APathNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	RootComponent = StaticMeshComp;

	Weight = 1;
	Graph = nullptr;
	SetCameFrom(nullptr);
}

// Called when the game starts or when spawned
void APathNode::BeginPlay()
{
	Super::BeginPlay();
	
}

void APathNode::UpdateNeighbours()
{
	switch (NodeType)
	{
	case EPathNodeType::PN_Undefined:
		break;
	case EPathNodeType::PN_Open:
		if (GraphLocation.y < Graph->SquareSize)
		{
			if (Graph->Grid[GraphLocation.x][GraphLocation.y + 1]->GetNodeType() != EPathNodeType::PN_Closed)
				AddNeighbourNode(Graph->Grid[GraphLocation.x][GraphLocation.y + 1]);
			if( Graph->bAllowDiagonalMovement && GraphLocation.x < Graph->SquareSize)
				if(Graph->Grid[GraphLocation.x + 1][GraphLocation.y + 1]->GetNodeType() != EPathNodeType::PN_Closed)
				AddNeighbourNode(Graph->Grid[GraphLocation.x + 1][GraphLocation.y + 1]);
		}
		if (GraphLocation.x < Graph->SquareSize)
		{
			if (Graph->Grid[GraphLocation.x + 1][GraphLocation.y]->GetNodeType() != EPathNodeType::PN_Closed)
				AddNeighbourNode(Graph->Grid[GraphLocation.x + 1][GraphLocation.y]);
			if (Graph->bAllowDiagonalMovement && GraphLocation.y > 0)
				if (Graph->Grid[GraphLocation.x + 1][GraphLocation.y - 1]->GetNodeType() != EPathNodeType::PN_Closed)
					AddNeighbourNode(Graph->Grid[GraphLocation.x + 1][GraphLocation.y - 1]);
		}
		if (GraphLocation.y > 0)
		{
			if (Graph->Grid[GraphLocation.x][GraphLocation.y - 1]->GetNodeType() != EPathNodeType::PN_Closed)
				AddNeighbourNode(Graph->Grid[GraphLocation.x][GraphLocation.y - 1]);
			if (Graph->bAllowDiagonalMovement && GraphLocation.x > 0)
				if (Graph->Grid[GraphLocation.x - 1][GraphLocation.y - 1]->GetNodeType() != EPathNodeType::PN_Closed)
					AddNeighbourNode(Graph->Grid[GraphLocation.x - 1][GraphLocation.y - 1]);
		}
		if (GraphLocation.x > 0)
		{
			if (Graph->Grid[GraphLocation.x - 1][GraphLocation.y]->GetNodeType() != EPathNodeType::PN_Closed)
				AddNeighbourNode(Graph->Grid[GraphLocation.x - 1][GraphLocation.y]);
			if (Graph->bAllowDiagonalMovement && GraphLocation.y < Graph->SquareSize)
				if (Graph->Grid[GraphLocation.x - 1][GraphLocation.y + 1]->GetNodeType() != EPathNodeType::PN_Closed)
					AddNeighbourNode(Graph->Grid[GraphLocation.x - 1][GraphLocation.y + 1]);
		}

		break;
	case EPathNodeType::PN_Closed:
		RemoveSelfFromNeighbourNodes();
		CameFrom = nullptr;
		break;
	default:
		break;
	}
}

FString APathNode::GetNodeTypeAsString(EPathNodeType EnumValue)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPathNodeType"), true);
	if (!EnumPtr) return FString("Invalid");

	return EnumPtr->GetNameByValue((int64)EnumValue).ToString();
}

void APathNode::UpdateNodeType()
{
	FVector loc = GetActorLocation();

	switch (NodeType)
	{
	case EPathNodeType::PN_Undefined:
		break;
	case EPathNodeType::PN_Open:
		loc.Z = 0.f;
		SetActorLocation(loc);
		if (OpenNodeMaterial)
			StaticMeshComp->SetMaterial(0, OpenNodeMaterial);
		break;
	case EPathNodeType::PN_Closed:
		loc.Z = 90.f;
		SetActorLocation(loc);
		CameFrom = nullptr;
		if(ClosedNodeMaterial)
			StaticMeshComp->SetMaterial(0, ClosedNodeMaterial);
		break;
	case EPathNodeType::PN_Start:
		Weight = 0;
		if (StartNodeMaterial)
			StaticMeshComp->SetMaterial(0, StartNodeMaterial);
		break;
	case EPathNodeType::PN_Finish:
		if (FinishNodeMaterial)
			StaticMeshComp->SetMaterial(0, FinishNodeMaterial);
		break;
	default:
		break;
	}
}

//// Called every frame
//void APathNode::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}