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


private:
	FIntPoint HeadCell = FIntPoint::ZeroValue;
	FIntPoint Direction = FIntPoint(1, 0);

	bool bDirectionChangedThisStep = false;

	void UpdateRotation(FIntPoint ChangeRotation);
	void UpdateHeadPosition();
	void SetApplePosition();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
