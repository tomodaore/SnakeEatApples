// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Snake.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"

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

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (!EnhancedInput || !PlayerController) {
		return;
	}

	if (!IsValid(SnakeMappingContext)
		|| !IsValid(UpAction)
		|| !IsValid(DownAction)
		|| !IsValid(LeftAction)
		|| !IsValid(RightAction)) {
		UE_LOG(LogTemp, Warning, TEXT("Snake input assets are not assigned."));
		return;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

	if (!LocalPlayer) {
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!InputSubsystem) {
		return;
	}

	InputSubsystem->AddMappingContext(SnakeMappingContext, 0);
	EnhancedInput->BindAction(UpAction, ETriggerEvent::Started, this, &ASnakeCameraPawn::TurnUp);
	EnhancedInput->BindAction(DownAction, ETriggerEvent::Started, this, &ASnakeCameraPawn::TurnDown);
	EnhancedInput->BindAction(LeftAction, ETriggerEvent::Started, this, &ASnakeCameraPawn::TurnLeft);
	EnhancedInput->BindAction(RightAction, ETriggerEvent::Started, this, &ASnakeCameraPawn::TurnRight);

	

}

void ASnakeCameraPawn::TurnUp() {
	if (IsValid(ControlledSnake) && ControlledSnake->RequestDirection(FIntPoint(1, 0))) {

		ControlledSnake->MoveOneCell();
	}
}
void ASnakeCameraPawn::TurnDown() {
	if (IsValid(ControlledSnake) && ControlledSnake->RequestDirection(FIntPoint(-1, 0))) {
	
		ControlledSnake->MoveOneCell();
	}
}
void ASnakeCameraPawn::TurnRight() {
	if (IsValid(ControlledSnake) && ControlledSnake->RequestDirection(FIntPoint(0, 1))) {
	
		ControlledSnake->MoveOneCell();
	}
}
void ASnakeCameraPawn::TurnLeft() {
	if (IsValid(ControlledSnake) && ControlledSnake->RequestDirection(FIntPoint(0,-1))) {
	
		ControlledSnake->MoveOneCell();
	}
}
