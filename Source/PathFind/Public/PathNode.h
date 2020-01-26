// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GraphSearchNode.h"
#include "PathNode.generated.h"

class UStaticMeshComponent;
class UMaterialInstance;

UENUM(BlueprintType)
enum EPathNodeType
{
	PN_Undefined UMETA(DisplayName="Undefined"),
	PN_Open UMETA(DisplayName="Open Node"),
	PN_Closed UMETA(DisplayName="Blocked Node"),
	PN_Start UMETA(DisplayName="Starting Node"),
	PN_Finish UMETA(DisplayName="Finish Node")
};

USTRUCT()
struct FGraphLocation
{
	GENERATED_BODY()

	UPROPERTY()
	int x;

	UPROPERTY()
	int y;
	
	FGraphLocation(int nx, int ny) : x(nx), y(ny) {}
	FGraphLocation() : x(-1), y(-1) {}

	void SetGraphLocation(int nx, int ny) { x = nx; y = ny; }
};

UCLASS()
class PATHFIND_API APathNode : public AGraphSearchNode
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInstance* OpenNodeMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInstance* ClosedNodeMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInstance* StartNodeMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInstance* FinishNodeMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	TEnumAsByte<EPathNodeType> NodeType;

	uint32 Weight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph")
	int CostSoFar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph")
	float DistToGraphFinish;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph")
	float Priority;

	UPROPERTY()
	FGraphLocation GraphLocation;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	UStaticMeshComponent* GetStaticMesh() { return StaticMeshComp;  }

	UPROPERTY()
	class AGridGraph* Graph;

	EPathNodeType GetNodeType() { return NodeType; }
	static FString GetNodeTypeAsString(EPathNodeType EnumValue);
	void SetNodeType(const EPathNodeType& newType) { NodeType = newType; }
	void UpdateNodeType();
	void UpdateNeighbours();

	FORCEINLINE FGraphLocation GetGraphLocation() const { return GraphLocation; }
	void SetGraphLocation(const FGraphLocation& Location) { GraphLocation.SetGraphLocation(Location.x, Location.y); }
};
