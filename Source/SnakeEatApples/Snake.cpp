// Fill out your copyright notice in the Description page of Project Settings.


#include "Snake.h"
#include "ASnakeBoard.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Apple.h"

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

	UpdateRotation(Direction);
	HeadCell = StartCell;
	UpdateHeadPosition();
	
}

void ASnake::MoveOneCell() {
	if(!IsValid(Board)) {
		return;
	}

	const FIntPoint NextCell = HeadCell + Direction;

	if (!Board->IsValidCell(NextCell)) {
		return;
	}
	if (bDirectionChangedThisStep) {
		UpdateRotation(Direction);
	}
	HeadCell = NextCell;
	UpdateHeadPosition();
	bDirectionChangedThisStep = false;

	if (IsValid(Apple) && HeadCell == Apple->GetCell()) {
		UE_LOG(LogTemp, Warning, TEXT("Snake ate the Apple."));
		SetApplePosition(HeadCell);
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

	if (!Board->IsValidCell(HeadCell + Direction)) {
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

void ASnake::SetApplePosition() {
	Apple->SetApplePosition();
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

