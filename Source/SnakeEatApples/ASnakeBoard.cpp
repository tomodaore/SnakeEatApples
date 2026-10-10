// Fill out your copyright notice in the Description page of Project Settings.


#include "ASnakeBoard.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AASnakeBoard::AASnakeBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

}

// Called when the game starts or when spawned
void AASnakeBoard::BeginPlay()
{
	Super::BeginPlay();

	if (bShowDebugGrid) {
		DrawDebugGrid();
	}
	
}

FVector AASnakeBoard::CellToWorld(FIntPoint Cell) const
{
	const FVector LocalPosition(Cell.X * CellSize, Cell.Y * CellSize, 0.0f);
	return GetActorTransform().TransformPosition(LocalPosition);
}

bool AASnakeBoard::IsValidCell(FIntPoint Cell) const
{
	return Cell.X >= 0 
		&& Cell.X < Columns 
		&& Cell.Y >= 0 
		&& Cell.Y < Rows;
}

void AASnakeBoard::DrawDebugGrid() const
{
	const float MinX = -CellSize * 0.5f;
	const float MinY = -CellSize * 0.5f;

	const float MaxX = MinX + Columns * CellSize;
	const float MaxY = MinY + Rows * CellSize;

	const float DebugHeight = 2.0f;
	
	const FTransform BoardTransform = GetActorTransform();
	for (int32 X = 0; X <= Columns; X++) {
		const float LineX = MinX + X * CellSize;
		const FVector Start = BoardTransform.TransformPosition(FVector(LineX, MinY, DebugHeight));
		const FVector End = BoardTransform.TransformPosition(FVector(LineX, MaxY, DebugHeight));
		DrawDebugLine(GetWorld(), Start, End, FColor::Black, true, -1.0f, 0, 2.0f);

	}

	for (int32 Y = 0; Y <= Rows; Y++) {
		const float LineY = MinY + Y * CellSize;
		const FVector Start = BoardTransform.TransformPosition(FVector(MinX, LineY, DebugHeight));
		const FVector End = BoardTransform.TransformPosition(FVector(MaxX, LineY, DebugHeight));
		DrawDebugLine(GetWorld(), Start, End, FColor::Black, true, -1.0f, 0, 2.0f);


	}

}

// Called every frame
void AASnakeBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

