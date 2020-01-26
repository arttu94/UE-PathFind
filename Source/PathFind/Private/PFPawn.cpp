// Fill out your copyright notice in the Description page of Project Settings.


#include "PFPawn.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/StaticMeshComponent.h"
#include "PathNode.h"
#include "GridGraph.h"
#include "Engine/World.h"

#include "EngineUtils.h"

#include "GraphSearch.h"

#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"

// Sets default values
APFPawn::APFPawn() : CurrentNode(nullptr), LastNode(nullptr)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement Component"));

	HitBrushType = EPathNodeType::PN_Undefined;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;

	bRightMouseButtonDown = false;
}

// Called when the game starts or when spawned
void APFPawn::BeginPlay()
{
	Super::BeginPlay();

	HitBrushType = EPathNodeType::PN_Undefined;

	if (GetController())
	{
		APlayerController* controller = Cast<APlayerController>(GetController());

		controller->bEnableMouseOverEvents = true;
		controller->bEnableClickEvents = true;
	}

	for (TActorIterator<AGridGraph> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		Graph = *ActorItr;
	}
}

void APFPawn::MoveForward(float value)
{
	MovementComp->AddInputVector(GetActorForwardVector() * value);
}

void APFPawn::MoveRight(float value)
{
	MovementComp->AddInputVector(GetActorRightVector() * value);
}

void APFPawn::CameraPitch(float value)
{
	if (bRightMouseButtonDown)
		AddControllerPitchInput(value);
}

void APFPawn::CameraYaw(float value)
{
	if (bRightMouseButtonDown)
		AddControllerYawInput(value);
}

void APFPawn::MouseCamBegin()
{
	bRightMouseButtonDown = true;
	if (GetController())
	{
		if (APlayerController* controller = Cast<APlayerController>(GetController()))
		{
			FInputModeGameOnly InputMode;
			controller->SetInputMode(InputMode);
			controller->bShowMouseCursor = false;
		}
	}
}

void APFPawn::MouseCamEnd()
{
	bRightMouseButtonDown = false;
	if (GetController())
	{
		if (APlayerController* controller = Cast<APlayerController>(GetController()))
		{
			FInputModeGameAndUI InputMode;
			InputMode.SetHideCursorDuringCapture(false);
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			controller->SetInputMode(InputMode);
			controller->bShowMouseCursor = true;
		}
	}
}

void APFPawn::MouseBrushBegin()
{
	bLeftMouseButtonDowm = true;
}

void APFPawn::MouseBrushEnd()
{
	bLeftMouseButtonDowm = false;
	LastNode = nullptr;
	HitBrushType = EPathNodeType::PN_Undefined;
}

// Called every frame
void APFPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bLeftMouseButtonDowm)
	{
		if (GetController())
		{
			APlayerController* controller = Cast<APlayerController>(GetController());

			FVector WorldLoc, WorldDir;

			controller->DeprojectMousePositionToWorld(WorldLoc, WorldDir);

			//DrawDebugLine(GetWorld(), WorldLoc, WorldLoc + (WorldDir * 10000), FColor::Red, false, 2.0f, (uint8)'\000', 1.0f);
			FHitResult Hit;
			if (GetWorld()->LineTraceSingleByChannel(
				Hit, // Out hit result
				WorldLoc, // Start
				WorldLoc + (WorldDir * 10000), // End
				ECollisionChannel::ECC_Visibility //trace channel
			))
			{
				if (APathNode* HitNode = Cast<APathNode>(Hit.Actor))
				{
					CurrentNode = HitNode;

					if (CurrentNode != LastNode)
					{

						LastNode = CurrentNode;
						CurrentNode = HitNode;

						if (HitBrushType.GetValue() == EPathNodeType::PN_Undefined)
						{
							HitBrushType = CurrentNode->GetNodeType();
							GEngine->AddOnScreenDebugMessage(0, 0.25f, FColor::Magenta, APathNode::GetNodeTypeAsString(LastNode->GetNodeType()));
						}

						switch (HitBrushType.GetValue())
						{
						case EPathNodeType::PN_Open:
							CurrentNode->SetNodeType(EPathNodeType::PN_Closed);
							break;
						case EPathNodeType::PN_Closed:
							CurrentNode->SetNodeType(EPathNodeType::PN_Open);
							break;
						}

						CurrentNode->UpdateNodeType();
						CurrentNode->UpdateNeighbours();
						Graph->ClearGraphNodes();
						//UGraphSearch::BreadthFirstSearch(Graph->Start);
						//Graph->DijkstraSearch(Graph->Start, Graph->Finish);
						//Graph->GreedyBestFirstSearch(Graph->Start, Graph->Finish);
						//Graph->AStarSearch(Graph->Start, Graph->Finish);
					}
				}
			}
		}
	}

}

// Called to bind functionality to input
void APFPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APFPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APFPawn::MoveRight);

	PlayerInputComponent->BindAxis("CameraPitch", this, &APFPawn::CameraPitch);
	PlayerInputComponent->BindAxis("CameraYaw", this, &APFPawn::CameraYaw);
	
	PlayerInputComponent->BindAction("MouseCam", IE_Pressed, this, &APFPawn::MouseCamBegin);
	PlayerInputComponent->BindAction("MouseCam", IE_Released, this, &APFPawn::MouseCamEnd);
	PlayerInputComponent->BindAction("MouseBrush", IE_Pressed, this, &APFPawn::MouseBrushBegin);
	PlayerInputComponent->BindAction("MouseBrush", IE_Released, this, &APFPawn::MouseBrushEnd);
}

