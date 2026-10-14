// Fill out your copyright notice in the Description page of Project Settings.


#include "Apple.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ASnakeBoard.h"

// Sets default values
AApple::AApple()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// SceneCompornetÇRootComponentÇ…ê›íËÇ∑ÇÈ
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	AppleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AppleMesh"));
	AppleMesh->SetupAttachment(DefaultSceneRoot);
}

// Called when the game starts or when spawned
void AApple::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(Board)) {
		UE_LOG(LogTemp, Warning, TEXT("Apple:Board is not assigned."));
		return;
	}

	
}

void AApple::SetApplePosition(const TArray<FIntPoint>& OccupiedCells) {

	if (!IsValid(Board)) {
		return;
	}
	AppleCell = Board->GetRandomCell(OccupiedCells);
	if (!Board->IsValidCell(AppleCell)) {
		SetActorHiddenInGame(true);
		UE_LOG(LogTemp, Warning, TEXT("No free cell remains."));
		return;
	}

	SetActorHiddenInGame(false);
	const FVector Position = Board->CellToWorld(AppleCell) + Board->GetActorUpVector() * AppleHeight;
	SetActorLocation(Position);
	return;
}

// Called every frame
void AApple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float RotationAmount = RotationSpeed * DeltaTime;
	AppleMesh->AddLocalRotation(FRotator(0.0f, RotationAmount, 0.0f));

}

