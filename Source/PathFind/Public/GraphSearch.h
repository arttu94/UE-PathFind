// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GraphSearch.generated.h"

/**
 * 
 */
UCLASS()
class PATHFIND_API UGraphSearch : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static void BreadthFirstSearch(class AGraphSearchNode* begin, AGraphSearchNode* end);

	UFUNCTION(BlueprintCallable)
	static void GreedyBestFirstSearch(AGraphSearchNode* begin, AGraphSearchNode* end);

	//Custom heuristics
	template <typename predicate>
	static void GreedyBestFirstSearch_H(AGraphSearchNode* begin, AGraphSearchNode* end, predicate heuristic);

	UFUNCTION(BlueprintCallable)
	static void DijkstraSearch(AGraphSearchNode* begin, AGraphSearchNode* end);

	UFUNCTION(BlueprintCallable)
	static void AStarSearch(AGraphSearchNode* begin, AGraphSearchNode* end);

};