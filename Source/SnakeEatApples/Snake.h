// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Snake.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class AASnakeBoard;
class FString;
class AApple;

UCLASS()
class SNAKEEATAPPLES_API ASnake : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnake();

	bool RequestDirection(FIntPoint NewDirection);
	void MoveOneCell();

	// For Test
	FIntPoint GetHeadCell() const { return HeadCell; }
	int32 GetLength() const { return SnakeCells.Num(); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> HeadMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Board")
	TObjectPtr<AASnakeBoard> Board;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Snake")
	FIntPoint StartCell = FIntPoint(0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Snake")
	float HeadHeight = 50.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Snake")
	TObjectPtr<AApple> Apple;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInstancedStaticMeshComponent> BodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Snake")
	float BodyScale = 0.8f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	
	TObjectPtr<UInstancedStaticMeshComponent> BodyMeshCurveLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	
	TObjectPtr<UInstancedStaticMeshComponent> BodyMeshCurveRight;


private:
	FIntPoint HeadCell = FIntPoint::ZeroValue;
	FIntPoint Direction = FIntPoint(1, 0);

	bool bDirectionChangedThisStep = false;

	void UpdateRotation(FIntPoint ChangeRotation);
	void UpdateHeadPosition();

	TArray<FIntPoint> SnakeCells;
	bool CanEnterCell(FIntPoint NextCell) const;
	void UpdateBodyMeshes();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
