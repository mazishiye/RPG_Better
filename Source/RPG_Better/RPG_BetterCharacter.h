// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGCharacterBase.h"
#include "GameFramework/Character.h"
#include "RPG_BetterCharacter.generated.h"

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None, // 0
	Confirm, // 用于确认操作
	Cancel, // 用于取消操作
	Ability1, // 技能1，例如普通攻击
	Ability2, // 技能2
	Ability3, // 技能3
};
/**
 *  A controllable top-down perspective character
 */
UCLASS(abstract)
class ARPG_BetterCharacter : public ARPGCharacterBase
{
	GENERATED_BODY()

private:

	// /** Top down camera */
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	// class UCameraComponent* TopDownCameraComponent;
	//
	// /** Camera boom positioning the camera above the character */
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	// class USpringArmComponent* CameraBoom;

public:

	/** Constructor */
	ARPG_BetterCharacter();

	/** Initialization */
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	/** Update */
	virtual void Tick(float DeltaSeconds) override;

	// /** Returns the camera component **/
	// FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	//
	// /** Returns the Camera Boom component **/
	// FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

};

