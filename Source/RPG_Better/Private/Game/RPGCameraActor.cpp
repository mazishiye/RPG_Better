// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RPGCameraActor.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ARPGCameraActor::ARPGCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(SceneRoot);
	SpringArm->TargetArmLength = 800.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 10.f;
	SpringArm->SetRelativeRotation(FRotator(0.f, 280.f, 0.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	ScrollSpeed = 1000.0f;
	EdgeScrollThreshold = 50.0f;
	ZoomSpeed = 100.0f;
	MinZoomLength = 500.0f;
	MaxZoomLength = 2000.0f;
	bEnableEdgeScroll = true;
	bIsMiddleMouseButtonDown = false;
}

// Called when the game starts or when spawned
void ARPGCameraActor::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		//PlayerController->SetViewTarget(this);
		PlayerController->SetViewTargetWithBlend(this, 0.5f);
	}
}

// Called every frame
void ARPGCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bEnableEdgeScroll)
	{
		HandleEdgeScroll();
		return;
	}
	if (FollowTarget && Target)
	{
		SetActorLocation(Target->GetActorLocation());
		return;
	}
	if (bIsMiddleMouseButtonDown)
	{
		HandleMiddleMouseDrag();
	}
}

void ARPGCameraActor::HandleEdgeScroll()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	UGameViewportClient* VC = GetWorld()->GetGameViewport();
	if (!VC) return;

	if (FVector2D MousePosition; PC->GetMousePosition(MousePosition.X, MousePosition.Y))
	{
		FVector2D ViewportSize;
		VC->GetViewportSize(ViewportSize);

		if (MousePosition.X < EdgeScrollThreshold)
		{
			AddActorLocalOffset(FVector(0.f, -ScrollSpeed * GetWorld()->GetDeltaSeconds(), 0.f));
		}
		else if (MousePosition.X > ViewportSize.X - EdgeScrollThreshold)
		{
			AddActorLocalOffset(FVector(0.f, ScrollSpeed * GetWorld()->GetDeltaSeconds(), 0.f));
		}
		else if (MousePosition.Y < EdgeScrollThreshold)
		{
			AddActorLocalOffset(FVector(ScrollSpeed * GetWorld()->GetDeltaSeconds(), 0.f, 0.f));
		}
		else if (MousePosition.Y > ViewportSize.Y - EdgeScrollThreshold)
		{
			AddActorLocalOffset(FVector(-ScrollSpeed * GetWorld()->GetDeltaSeconds(), 0.f, 0.f));
		}
	}
}

void ARPGCameraActor::ZoomIn()
{
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength - ZoomSpeed, MinZoomLength, MaxZoomLength);
}

void ARPGCameraActor::ZoomOut()
{
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength + ZoomSpeed, MinZoomLength, MaxZoomLength);
}

void ARPGCameraActor::HandleMiddleMouseDrag()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	FVector2D CurrentMousePosition;
	if (PC->GetMousePosition(CurrentMousePosition.X, CurrentMousePosition.Y))
	{
		FVector2D Delta = CurrentMousePosition - LastMousePosition;
		AddActorLocalOffset(FVector(
			Delta.Y * ScrollSpeed * GetWorld()->GetDeltaSeconds(),
			-Delta.X * ScrollSpeed * GetWorld()->GetDeltaSeconds(),
			0.f));
	}
	LastMousePosition = CurrentMousePosition;
}

void ARPGCameraActor::StartMiddleMouseDrag()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	PC->GetMousePosition(LastMousePosition.X, LastMousePosition.Y);
	bIsMiddleMouseButtonDown = true;
}

void ARPGCameraActor::EndMiddleMouseDrag()
{
	bIsMiddleMouseButtonDown = false;
}
