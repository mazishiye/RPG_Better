// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGCameraActor.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class RPG_BETTER_API ARPGCameraActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARPGCameraActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float ScrollSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float EdgeScrollThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float ZoomSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float MinZoomLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float MaxZoomLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	bool bEnableEdgeScroll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	bool FollowTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	AActor* Target;
public:
	void HandleEdgeScroll();
	void ZoomIn();
	void ZoomOut();
	
	void HandleMiddleMouseDrag();
	void StartMiddleMouseDrag();
	void EndMiddleMouseDrag();
	
	UPROPERTY()
	bool bIsMiddleMouseButtonDown;
	UPROPERTY()
	FVector2D LastMousePosition;
};
