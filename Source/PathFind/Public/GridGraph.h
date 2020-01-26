// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridGraph.generated.h"

class APathNode;
class AGraphSearchNode;
struct FGraphLocation;

UCLASS()
class PATHFIND_API AGridGraph : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridGraph();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Node Spawning")
	TSubclassOf<APathNode> NodeBlueprintClass;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	const int32 SquareSize = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path Finding")
	bool bAllowDiagonalMovement;

	TArray<TArray<APathNode*>> Grid;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Path Finding")
	APathNode* Start;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Path Finding")
	APathNode* Finish;

	UFUNCTION(BlueprintCallable)
	void ClearGraphNodes();
};
