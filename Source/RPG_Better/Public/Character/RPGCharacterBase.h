// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "RPGCharacterBase.generated.h"

class URPGAbilitySystemComponent;

UCLASS()
class RPG_BETTER_API ARPGCharacterBase : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	
	ARPGCharacterBase();

	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	
	

	TWeakObjectPtr<URPGAbilitySystemComponent> AbilitySystemComponent;

public:	
	virtual void OnRep_PlayerState() override;
};
