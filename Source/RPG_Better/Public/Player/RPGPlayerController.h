// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GAS/RPGAbilitySystemComponent.h"
#include "RPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPG_BETTER_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	
	TWeakObjectPtr<URPGAbilitySystemComponent> AbilitySystemComponent;
};
