// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_BetterPlayerController.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "RPG_BetterCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "RPG_Better.h"
#include "Game/RPGCameraActor.h"
#include "Player/RPGPlayerState.h"

ARPG_BetterPlayerController::ARPG_BetterPlayerController()
{
	bIsTouch = false;
	bMoveToMouseCursor = false;

	// configure the controller
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void ARPG_BetterPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Only set up input on local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		// Set up action bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			// Setup mouse input events
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this,
			                                   &ARPG_BetterPlayerController::OnInputStarted);
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this,
			                                   &ARPG_BetterPlayerController::OnSetDestinationTriggered);
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this,
			                                   &ARPG_BetterPlayerController::OnSetDestinationReleased);
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this,
			                                   &ARPG_BetterPlayerController::OnSetDestinationReleased);
			EnhancedInputComponent->BindAction(MiddleMouseDragAction, ETriggerEvent::Started, this,
			                                   &ARPG_BetterPlayerController::OnMiddleMouseDragTriggered);
			EnhancedInputComponent->BindAction(MiddleMouseDragAction, ETriggerEvent::Completed, this,
			                                   &ARPG_BetterPlayerController::OnMiddleMouseDragReleased);
			EnhancedInputComponent->BindAction(CameraZoomAction, ETriggerEvent::Triggered, this,
			                                   &ARPG_BetterPlayerController::OnCameraZoomTriggered);

			// Setup touch input events
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this,
			                                   &ARPG_BetterPlayerController::OnInputStarted);
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this,
			                                   &ARPG_BetterPlayerController::OnTouchTriggered);
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this,
			                                   &ARPG_BetterPlayerController::OnTouchReleased);
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this,
			                                   &ARPG_BetterPlayerController::OnTouchReleased);

			//Set up ability input events
			EnhancedInputComponent->BindAction(AbilityConfirmAction,ETriggerEvent::Started,this,&ARPG_BetterPlayerController::OnConfirmAction);
			EnhancedInputComponent->BindAction(AbilityCancelAction,ETriggerEvent::Started,this,&ARPG_BetterPlayerController::OnCancelAction);
		}
		else
		{
			UE_LOG(LogRPG_Better, Error,
			       TEXT(
				       "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
			       ), *GetNameSafe(this));
		}
	}	
}

void ARPG_BetterPlayerController::OnInputStarted()
{
	StopMovement();
}

void ARPG_BetterPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void ARPG_BetterPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator,
		                                               FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void ARPG_BetterPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void ARPG_BetterPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void ARPG_BetterPlayerController::OnMiddleMouseDragTriggered()
{
	CameraActor->StartMiddleMouseDrag();
}

void ARPG_BetterPlayerController::OnMiddleMouseDragReleased()
{
	CameraActor->EndMiddleMouseDrag();
}

void ARPG_BetterPlayerController::OnCameraZoomTriggered(const FInputActionValue& Value)
{
	if (Value.Get<float>() > 0)
	{
		CameraActor->ZoomOut();
	}
	else
	{
		CameraActor->ZoomIn();
	}
}

void ARPG_BetterPlayerController::OnConfirmAction()
{
	if(AbilitySystemComponent.IsValid())
		AbilitySystemComponent.Get()->AbilityLocalInputPressed(static_cast<int32>(EAbilityInputID::Confirm));
}

void ARPG_BetterPlayerController::OnCancelAction()
{
	if(AbilitySystemComponent.IsValid())
		AbilitySystemComponent.Get()->AbilityLocalInputPressed(static_cast<int32>(EAbilityInputID::Cancel));
}
