// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GraphSearchNode.generated.h"

UCLASS()
class PATHFIND_API AGraphSearchNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGraphSearchNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph Search")
	float NodePriority;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph Search")
	float NodeWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph Search")
	TWeakObjectPtr<AGraphSearchNode> CameFrom;

	TArray<TWeakObjectPtr<AGraphSearchNode>> NeighbourNodes;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//weak pointers because maybe we want to delete nodes from the grid? a non regular grid maybe? 
	//might go back to regular AGraphSearchNode* if it's too much of a hassle with little to no gain.

	FORCEINLINE TArray<TWeakObjectPtr<AGraphSearchNode>>& GetNeighbourNodes() { return NeighbourNodes; }
	FORCEINLINE void AddNeighbourNode(AGraphSearchNode* nodeToAdd, bool AddSelfToNeighbour = true) 
	{ 
		NeighbourNodes.Add(MakeWeakObjectPtr(nodeToAdd)); 
		if(AddSelfToNeighbour)
			nodeToAdd->AddNeighbourNode(this, false); // set add self param to false to avoid infinite recursion.
	}
	FORCEINLINE int32 RemoveNeighbourNode(AGraphSearchNode* nodeToRemove) 
	{
		if (NeighbourNodes.Contains(nodeToRemove))
			return NeighbourNodes.RemoveSwap(nodeToRemove);
		return 0;
	}
	FORCEINLINE void EmptyNeighbourNodes() 
	{
		for (auto neighbour : NeighbourNodes)
		{
			//TODO: check if this is needed, probably not since TArray::Empty() calls the destructor of the objects
			neighbour = nullptr;
		}
		NeighbourNodes.Empty();
	}
	FORCEINLINE void RemoveSelfFromNeighbourNodes(bool ShouldEmptyNeighbourNodes = true)
	{ 
		for (auto neighbour : NeighbourNodes)
		{
			if (neighbour.IsValid()) // non-nllptr 
				if(neighbour.Get()->NeighbourNodes.Contains(this)) // make sure this is in the neighbour neighours 
					neighbour.Get()->NeighbourNodes.RemoveSwap(this); // optimal way of removing self/this since we don't care about neighbour order
		}
		if (ShouldEmptyNeighbourNodes)
			EmptyNeighbourNodes();
	}

	FORCEINLINE void SetNodePriority(float val) { NodePriority = val; }
	FORCEINLINE float GetNodePriority() const { return NodePriority; }	
	
	FORCEINLINE void SetNodeWeight(float val) { NodeWeight = val; }
	FORCEINLINE float GetNodeWeight() const { return NodeWeight; }

	FORCEINLINE TWeakObjectPtr<AGraphSearchNode> GetCameFrom() const { return CameFrom; }
	FORCEINLINE void SetCameFrom(AGraphSearchNode* val) { CameFrom = MakeWeakObjectPtr(val); }
};
