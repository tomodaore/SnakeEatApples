// Fill out your copyright notice in the Description page of Project Settings.


#include "Snake.h"
#include "ASnakeBoard.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Apple.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
ASnake::ASnake()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(SceneRoot);

	HeadMesh->SetMobility(EComponentMobility::Movable);
	HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BodyMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(SceneRoot);
	BodyMesh->SetMobility(EComponentMobility::Movable);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BodyMeshCurveRight = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BodyMeshCurveRight"));
	BodyMeshCurveRight->SetupAttachment(SceneRoot);
	BodyMeshCurveRight->SetMobility(EComponentMobility::Movable);
	BodyMeshCurveRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BodyMeshCurveLeft = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BodyMeshCurveLeft"));
	BodyMeshCurveLeft->SetupAttachment(SceneRoot);
	BodyMeshCurveLeft->SetMobility(EComponentMobility::Movable);
	BodyMeshCurveLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}

// Called when the game starts or when spawned
void ASnake::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(Board)) {
		UE_LOG(LogTemp, Warning, TEXT("Snake: Board is not assigned"));
		return;
	}

	if(!Board->IsValidCell(StartCell)) {
		UE_LOG(LogTemp, Warning, TEXT("Snake: StartCell is not valid"));
		return;
	}
	if (!IsValid(Apple)) {
		UE_LOG(LogTemp, Warning, TEXT("Snake: Apple is not valid."));
		return;
	}

	UpdateRotation(Direction);
	HeadCell = StartCell;
	SnakeCells.Reset();
	SnakeCells.Add(HeadCell);
	UpdateHeadPosition();
	Apple->SetApplePosition(SnakeCells);
	
}

void ASnake::MoveOneCell() {
	if (!IsValid(Board) || SnakeCells.IsEmpty()) {
		return;
	}

	const FIntPoint NextCell = HeadCell + Direction;

	if (!CanEnterCell(NextCell)) {
		return;
	}

	const bool bAteApple = IsValid(Apple) && NextCell == Apple->GetCell();

	SnakeCells.Insert(NextCell, 0);

	if (!bAteApple) {
		SnakeCells.RemoveAt(SnakeCells.Num() - 1);
	}

	HeadCell = SnakeCells[0];

	UpdateRotation(Direction);
	UpdateHeadPosition();
	UpdateBodyMeshes();

	bDirectionChangedThisStep = false;
	if (bAteApple) {
		UE_LOG(LogTemp, Warning, TEXT("Snake length: %d"), SnakeCells.Num());
		Apple->SetApplePosition(SnakeCells);

	}
}

void ASnake::UpdateHeadPosition() {
	const FVector CellPosition = Board->CellToWorld(HeadCell);
	const FVector HeadPosition = CellPosition + Board->GetActorUpVector() * HeadHeight;
	SetActorLocation(HeadPosition);
	//UE_LOG(LogTemp, Warning, TEXT("Snake: HeadCell = (%s)"), *CellPosition.ToString());
}


bool ASnake::RequestDirection(FIntPoint NewDirection) {
	if (FMath::Abs(NewDirection.X) + FMath::Abs(NewDirection.Y) != 1) {
		return false;
	}

	const FIntPoint OppositeDirection(-Direction.X, -Direction.Y);
	if (NewDirection == OppositeDirection) {
		return false;
	}

	if (!IsValid(Board)) {
		return false;
	}

	if (!CanEnterCell(HeadCell + NewDirection)) {
		return false;
	}

	if (Direction != NewDirection) {
		bDirectionChangedThisStep = true;
	}

	Direction = NewDirection;
	return true;
}

void ASnake::UpdateRotation(FIntPoint ChangeRotation) {
	if (ChangeRotation == FIntPoint(1, 0)) {
		// Up
		SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
	else if (ChangeRotation == FIntPoint(-1, 0)) {
		// Down
		SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
	}
	else if (ChangeRotation == FIntPoint(0, 1)) {
		// Right
		SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
	}
	else {
		// Left
		SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
}

bool ASnake::CanEnterCell(FIntPoint NextCell) const {
	if (!IsValid(Board) || SnakeCells.IsEmpty()) {
		return false;
	}

	if (!Board->IsValidCell(NextCell)) {
		return false;
	}
	const bool bWillGrow = IsValid(Apple) && NextCell == Apple->GetCell();
	const int32 CheckEnd = SnakeCells.Num() - (bWillGrow ? 0 : 1);

	for (int32 Index = 1; Index < CheckEnd; Index++) {
		if (SnakeCells[Index] == NextCell) {
			return false;
		}
	}
	return true;
}

void ASnake::UpdateBodyMeshes() {
	BodyMesh->ClearInstances();
	BodyMeshCurveLeft->ClearInstances();
	BodyMeshCurveRight->ClearInstances();
	bool bStraight;
	for (int32 Index = 1; Index < SnakeCells.Num(); Index++) {
		const FIntPoint CurrentCell = SnakeCells[Index];
		const FIntPoint FrontCell = SnakeCells[Index - 1];
		const FIntPoint CellDirection = FrontCell - CurrentCell;
		FIntPoint BackCell;
		const FVector LocalDirection(CellDirection.X, CellDirection.Y, 0.0f);
		if (Index != SnakeCells.Num() - 1) {
			BackCell = SnakeCells[Index + 1];
			bStraight = (FrontCell - CurrentCell) + (BackCell - CurrentCell)== FIntPoint::ZeroValue;
		}
		else {
			bStraight = true;
		}
		FRotator BodyRotation = LocalDirection.Rotation();
		const FVector Position = Board->CellToWorld(CurrentCell) + Board->GetActorUpVector() * HeadHeight;

		if (bStraight) {
			BodyRotation.Yaw -= 90.0f;
			const FTransform BodyTransform(BodyRotation, Position, FVector(BodyScale));
			BodyMesh->AddInstance(BodyTransform, true);
		}
		else {
			const FIntPoint Incoming = CurrentCell - BackCell;
			const FIntPoint Outgoing = FrontCell - CurrentCell;
			const int32 Cross = Incoming.X * Outgoing.Y - Incoming.Y * Outgoing.X;
			if (Cross < 0) {
				// Left
				const FTransform BodyTransform(BodyRotation, Position, FVector(BodyScale));
				BodyMeshCurveLeft->AddInstance(BodyTransform, true);
			}
			else {
				// Right
				BodyRotation.Yaw += 180.0f;
				const FTransform BodyTransform(BodyRotation, Position, FVector(BodyScale));
				BodyMeshCurveRight->AddInstance(BodyTransform, true);
			}
		}
	}
}

//void ASnake::EndPlay(const EEndPlayReason::Type EndPlayReason) {
//	GetWorldTimerManager().ClearTimer(MoveTimerHandle);
//	Super::EndPlay(EndPlayReason);
//
//}
// Called every frame
void ASnake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

