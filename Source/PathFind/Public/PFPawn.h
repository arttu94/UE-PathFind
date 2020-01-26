// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PFPawn.generated.h"

class UFloatingPawnMovement;
class APathNode;
class AGridGraph;
enum EPathNodeType;

UCLASS()
class PATHFIND_API APFPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APFPawn();

protected:
	UPROPERTY()
	UFloatingPawnMovement* MovementComp;

	bool bRightMouseButtonDown;
	bool bLeftMouseButtonDowm;

	UPROPERTY()
	APathNode* LastNode;
	UPROPERTY()
	APathNode* CurrentNode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AGridGraph* Graph;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	TEnumAsByte<EPathNodeType> HitBrushType;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);

	void CameraPitch(float value);
	void CameraYaw(float value);

	void MouseCamBegin();
	void MouseCamEnd();

	void MouseBrushBegin();
	void MouseBrushEnd();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
