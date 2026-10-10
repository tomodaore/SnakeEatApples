// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
ASnakeCameraPawn::ASnakeCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SceneRoot);

	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	Camera->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	Camera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	Camera->SetOrthoWidth(1400.0f);

	Camera->bUsePawnControlRotation = false;



}

// Called when the game starts or when spawned
void ASnakeCameraPawn::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(BoardFloor)) {
		UE_LOG(LogTemp, Warning, TEXT("BoardFloor is not set in SnakeCameraPawn. Please assign a valid board actor."));
		return;
	}

	FVector FloorCenter;
	FVector FloorExtent;

	BoardFloor->GetActorBounds(false, FloorCenter, FloorExtent);

	const FVector FloorTopCenter{FloorCenter.X,
		FloorCenter.Y,
		FloorCenter.Z + FloorExtent.Z
	};

	SetActorLocation(FloorTopCenter);
	SetActorRotation(FRotator::ZeroRotator);
	SetActorScale3D(FVector::OneVector);

	SceneRoot->SetAbsolute(false, false, true);

	AttachToActor(BoardFloor, FAttachmentTransformRules::KeepWorldTransform);
	
}

// Called every frame
void ASnakeCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASnakeCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

