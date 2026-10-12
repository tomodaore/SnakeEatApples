// Fill out your copyright notice in the Description page of Project Settings.


#include "Snake.h"
#include "ASnakeBoard.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

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


	HeadCell = StartCell;
	UpdateHeadPosition();

	// ŠJŽn“ñ•bŽ~‚Ü‚é
	//GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &ASnake::StartMoving, 2.0f, false);


	GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &ASnake::MoveOneCell, MoveInterval, true);


	
}

void ASnake::MoveOneCell() {
	if(!IsValid(Board)) {
		GetWorldTimerManager().ClearTimer(MoveTimerHandle);
		return;
	}

	const FIntPoint NextCell = HeadCell + Direction;

	if (!Board->IsValidCell(NextCell)) {
		GetWorldTimerManager().ClearTimer(MoveTimerHandle);
		return;
	}
	HeadCell = NextCell;
	UpdateHeadPosition();

	bDirectionChangedThisStep = false;
}

void ASnake::UpdateHeadPosition() {
	const FVector CellPosition = Board->CellToWorld(HeadCell);
	const FVector HeadPosition = CellPosition + Board->GetActorUpVector() * HeadHeight;
	SetActorLocation(HeadPosition);
	UE_LOG(LogTemp, Warning, TEXT("Snake: HeadCell = (%s)"), *CellPosition.ToString());
}

void ASnake::StartMoving() {
	
	GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &ASnake::MoveOneCell, MoveInterval, true);
}

void ASnake::RequestDirection(FIntPoint NewDirection) {
	if (FMath::Abs(NewDirection.X) + FMath::Abs(NewDirection.Y) != 1) {
		return;
	}

	if (bDirectionChangedThisStep) {
		return;
	}

	if (NewDirection == Direction) {
		return;
	}

	const FIntPoint OppositeDirection(-Direction.X, -Direction.Y);
	if (NewDirection == OppositeDirection) {
		return;
	}

	Direction = NewDirection;
	bDirectionChangedThisStep = true;
}

void ASnake::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	GetWorldTimerManager().ClearTimer(MoveTimerHandle);
	Super::EndPlay(EndPlayReason);

}
// Called every frame
void ASnake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

