// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphSearch.h"
#include "GraphSearchNode.h"
#include "Containers/Queue.h"
#include <queue>

void UGraphSearch::BreadthFirstSearch(AGraphSearchNode* begin, AGraphSearchNode* end = nullptr)
{
	TQueue<AGraphSearchNode*> frontier;
	frontier.Enqueue(begin);
	TMap<AGraphSearchNode*, bool> visited;

	begin->SetCameFrom(nullptr);
	visited.Emplace(begin, true);

	while (!frontier.IsEmpty())
	{
		auto current = *frontier.Peek();
		frontier.Pop();

		//if an end node is provided we do the early exit
		if (end)
			if (current == end)
				break;

		for (auto next : current->GetNeighbourNodes())
		{
			if (next.IsValid())
			{
				if (!visited.Contains(next.Get()))
				{
					frontier.Enqueue(next.Get());
					next.Get()->SetCameFrom(current);
					visited.Emplace(next.Get(), true);
				}
			}
		}
	}
}

void UGraphSearch::GreedyBestFirstSearch(AGraphSearchNode* begin, AGraphSearchNode* end)
{
	auto heapPredicate = [&end](const AGraphSearchNode& Lhs, const AGraphSearchNode& Rhs)
	{
		//Manhattan distance 
		//return (FMath::Abs(Lhs.GetActorLocation().X - end->GetActorLocation().X) + FMath::Abs(Lhs.GetActorLocation().Y - end->GetActorLocation().Y)) < (FMath::Abs(Rhs.GetActorLocation().X - end->GetActorLocation().X) + FMath::Abs(Rhs.GetActorLocation().Y - end->GetActorLocation().Y));
		//Diagonal Distance
		//return FMath::Max(FMath::Abs(Lhs.GetActorLocation().X - end->GetActorLocation().X), FMath::Abs(Lhs.GetActorLocation().Y - end->GetActorLocation().Y)) < FMath::Max(FMath::Abs(Rhs.GetActorLocation().X - end->GetActorLocation().X), FMath::Abs(Rhs.GetActorLocation().Y - end->GetActorLocation().Y));
		//Euclidean Distance 
		return FMath::Sqrt(FMath::Square(Lhs.GetActorLocation().X - end->GetActorLocation().X) + FMath::Square(Lhs.GetActorLocation().Y - end->GetActorLocation().Y)) < FMath::Sqrt(FMath::Square(Rhs.GetActorLocation().X - end->GetActorLocation().X) + FMath::Square(Rhs.GetActorLocation().Y - end->GetActorLocation().Y));
		//Cheap Euclidean
		//we avoid using sqrt so every value will be high but will be trated the same way during comparison
		//return (FMath::Square(Lhs.GetActorLocation().X - end->GetActorLocation().X) + FMath::Square(Lhs.GetActorLocation().Y - end->GetActorLocation().Y)) < (FMath::Square(Rhs.GetActorLocation().X - end->GetActorLocation().X) + FMath::Square(Rhs.GetActorLocation().Y - end->GetActorLocation().Y));
	};

	TArray<AGraphSearchNode*> frontier;
	frontier.Add(begin);
	begin->SetCameFrom(nullptr);
	TMap<AGraphSearchNode*, AGraphSearchNode*> cameFrom;
	cameFrom.Emplace(begin, nullptr);

	while (frontier.Num() > 0)
	{
		AGraphSearchNode* current = nullptr;
		frontier.HeapPop(current, heapPredicate, true);

		if (current == end)
			break;

		for (auto next : current->GetNeighbourNodes())
		{
			if (!cameFrom.Contains(next.Get()))
			{
				auto dist = FMath::Abs(next.Get()->GetActorLocation().X - end->GetActorLocation().X) + FMath::Abs(next.Get()->GetActorLocation().Y - end->GetActorLocation().Y);
				next.Get()->SetCameFrom(current);
				cameFrom.Emplace(next.Get(), current);
				frontier.HeapPush(next.Get(), heapPredicate);
			}
		}
	}
}

template<typename predicate>
void UGraphSearch::GreedyBestFirstSearch_H(AGraphSearchNode* begin, AGraphSearchNode* end, predicate heuristic)
{

}

void UGraphSearch::DijkstraSearch(AGraphSearchNode* begin, AGraphSearchNode* end)
{
	auto heapPredicate = [](const AGraphSearchNode& Lhs, const AGraphSearchNode& Rhs) { return Lhs.GetNodePriority() < Rhs.GetNodePriority(); };
	TArray<AGraphSearchNode*> frontier;
	frontier.Add(begin);
	TMap<AGraphSearchNode*, int> CostSoFar;

	begin->SetCameFrom(nullptr);
	begin->SetNodePriority(0);
	CostSoFar.Emplace(begin, 0);

	while (frontier.Num() > 0)
	{
		AGraphSearchNode* current = nullptr;
		frontier.HeapPop(current, heapPredicate, true);

		if (current == end)
			break;

		for (auto next : current->GetNeighbourNodes())
		{
			int newCost = CostSoFar[current] + next.Get()->GetNodeWeight();
			if (CostSoFar.Find(next.Get()) == nullptr || newCost < CostSoFar[next.Get()])
			{
				CostSoFar.Emplace(next.Get(), newCost);
				next.Get()->SetNodePriority(newCost);
				next.Get()->SetCameFrom(current);
				frontier.HeapPush(next.Get(), heapPredicate);
			}
		}
	}
}

void UGraphSearch::AStarSearch(AGraphSearchNode* begin, AGraphSearchNode* end)
{
	auto heuristic = [&end](const AGraphSearchNode& n)
	{
		//Manhattan Distance
		//return FMath::Abs(n.GetActorLocation().X - end->GetActorLocation().X) + FMath::Abs(n.GetActorLocation().Y - end->GetActorLocation().Y);
		//Diagonal Distance
		//return FMath::Max(FMath::Abs(n.GetActorLocation().X - end->GetActorLocation().X), FMath::Abs(n.GetActorLocation().Y - end->GetActorLocation().Y));
		//Euclidean Distance 
		return FMath::Sqrt(FMath::Square(n.GetActorLocation().X - end->GetActorLocation().X) + FMath::Square(n.GetActorLocation().Y - end->GetActorLocation().Y));
		//Cheap Euclidean
		//we avoid using sqrt so every value will be high but will be trated the same way during comparison
	};

	auto heapPredicate = [](const AGraphSearchNode& Lhs, const AGraphSearchNode& Rhs)
	{
		return Lhs.GetNodePriority() < Rhs.GetNodePriority();
	};

	TArray<AGraphSearchNode*> frontier;
	frontier.Add(begin);
	TMap<AGraphSearchNode*, float> costSofar;

	costSofar.Emplace(begin, 0);
	begin->SetNodePriority(0);

	while (frontier.Num() > 0)
	{
		AGraphSearchNode* current = nullptr;
		frontier.HeapPop(current, heapPredicate, true);

		if (current == end)
			break;

		for (auto next : current->GetNeighbourNodes())
		{
			auto newCost = costSofar[current] + next.Get()->GetNodeWeight();
			if (!costSofar.Find(next.Get()) || newCost < costSofar[next.Get()])
			{
				costSofar.Emplace(next.Get(), newCost);
				auto priority = newCost + heuristic(*next.Get());
				next.Get()->SetNodePriority(priority);
				next.Get()->SetCameFrom(current);
				frontier.HeapPush(next.Get(), heapPredicate);
			}
		}
	}

}