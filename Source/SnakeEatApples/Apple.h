// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Apple.generated.h"

class AASnakeBoard;

UCLASS()
class SNAKEEATAPPLES_API AApple : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AApple();
	FIntPoint GetCell() const { return AppleCell; }
	void SetApplePosition(const TArray<FIntPoint>& OccupiedCells);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Components")
	TObjectPtr<UStaticMeshComponent> AppleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apple")
	float RotationSpeed = 90.0f; // âÒì]ë¨ìxÅiìx/ïbÅj

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Board")
	TObjectPtr<AASnakeBoard> Board;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Apple")
	float AppleHeight = 50.0f;


private:
	FIntPoint AppleCell = FIntPoint::ZeroValue;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};