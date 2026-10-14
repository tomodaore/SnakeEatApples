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

	// For Test
	if (bAutoTest) {
		StartAutoTest();
	}
	
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

	// For Test
	if (bAutoTest) {
		return;
	}

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

// For Test
void ASnakeCameraPawn::StartAutoTest()
{
	if (!IsValid(ControlledSnake))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("Auto test: ControlledSnake is not assigned."));
		return;
	}

	// 今回は10×10盤面専用
	const int32 SizeX = 10;
	const int32 SizeY = 10;

	AutoRoute.Reset();

	// 最初は＋X方向へ進む
	for (int32 X = 0; X < SizeX; ++X)
	{
		AutoRoute.Add(FIntPoint(X, 0));
	}

	// X＝0を戻り道として残し、蛇行する
	for (int32 Y = 1; Y < SizeY; ++Y)
	{
		if (Y % 2 == 1)
		{
			for (int32 X = SizeX - 1; X >= 1; --X)
			{
				AutoRoute.Add(FIntPoint(X, Y));
			}
		}
		else
		{
			for (int32 X = 1; X < SizeX; ++X)
			{
				AutoRoute.Add(FIntPoint(X, Y));
			}
		}
	}

	// X＝0の列で出発点の手前まで戻る
	for (int32 Y = SizeY - 1; Y >= 1; --Y)
	{
		AutoRoute.Add(FIntPoint(0, Y));
	}

	GetWorldTimerManager().SetTimer(
		AutoMoveTimer,
		this,
		&ASnakeCameraPawn::AutoMoveOneCell,
		AutoStepInterval,
		true
	);
}

// For Test
void ASnakeCameraPawn::AutoMoveOneCell()
{
	if (!IsValid(ControlledSnake) || AutoRoute.IsEmpty())
	{
		GetWorldTimerManager().ClearTimer(AutoMoveTimer);
		return;
	}

	// 初期化されていなければ停止する
	if (ControlledSnake->GetLength() == 0)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("Auto test: Snake is not initialized."));

		GetWorldTimerManager().ClearTimer(AutoMoveTimer);
		return;
	}

	// 全マスまで伸びたらテスト終了
	if (ControlledSnake->GetLength() >= AutoRoute.Num())
	{
		UE_LOG(LogTemp, Warning,
			TEXT("Auto test completed: length=%d"),
			ControlledSnake->GetLength());

		GetWorldTimerManager().ClearTimer(AutoMoveTimer);
		return;
	}

	const FIntPoint CurrentCell =
		ControlledSnake->GetHeadCell();

	const int32 CurrentIndex = AutoRoute.IndexOfByKey(CurrentCell);

	if (CurrentIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("Auto test: Head is outside the route."));

		GetWorldTimerManager().ClearTimer(AutoMoveTimer);
		return;
	}

	// 最後まで来たら、次はルートの先頭
	const int32 NextIndex =
		(CurrentIndex + 1) % AutoRoute.Num();

	const FIntPoint TargetCell = AutoRoute[NextIndex];
	const FIntPoint MoveDirection = TargetCell - CurrentCell;

	if (!ControlledSnake->RequestDirection(MoveDirection))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("Auto test: Movement was rejected."));

		GetWorldTimerManager().ClearTimer(AutoMoveTimer);
		return;
	}

	ControlledSnake->MoveOneCell();

	if (ControlledSnake->GetHeadCell() != TargetCell)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("Auto test: Snake did not reach the target."));

		GetWorldTimerManager().ClearTimer(AutoMoveTimer);
	}
}
