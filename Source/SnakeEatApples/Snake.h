// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Snake.generated.h"
#include "TimerManager.h"

class USceneComponent;
class UStaticMeshComponent;
class AASnakeBoard;

UCLASS()
class SNAKEEATAPPLES_API ASnake : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnake();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> HeadMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Board")
	TObjectPtr<AASnakeBoard> Board;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Snake")
	FIntPoint StartCell = FIntPoint(0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Snake", meta = (ClampMin = "0.05"))
	float MovwInterval = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Snake")
	float HeadHeight = 50.0f;

private:
	FIntPoint HeadCell = FIntPoint::ZeroValue;
	FIntPoint Direction = FIntPoint(1, 0);

	FTimerHandle MoveTimerHandle;

	void MoveOneCell();
	void UpdateHeadPosition();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
