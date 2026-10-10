// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASnakeBoard.generated.h"

UCLASS()
class SNAKEEATAPPLES_API AASnakeBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASnakeBoard();

	UFUNCTION(BlueprintPure, Category = "Board")
	FVector CellToWorld(FIntPoint Cell) const;

	UFUNCTION(BlueprintPure, Category = "Board")
	bool IsValidCell(FIntPoint Cell) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board", meta = (ClampMin = "1", UIMin = "1"))
	int32 Columns = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board", meta = (ClampMin = "1", UIMin = "1"))
	int32 Rows = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board", meta = (ClampMin = "1",UIMin = "1"))
	float CellSize = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board|Debug")
	bool bShowDebugGrid = true;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void DrawDebugGrid() const;

};
