// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameplayEffect.h"
#include "AbilitySystemInterface.h"
#include "RPGPlayerState.generated.h"

class URPGAttributeSet;
class URPGAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class RPG_BETTER_API ARPGPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ARPGPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	URPGAttributeSet* GetAttributeSet() const;

	virtual  void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,Category="GameplayEffect")
	TSubclassOf<UGameplayEffect> AttributeEffect;
protected:

	bool InitAttribute();
	
	UPROPERTY()
	URPGAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY()
	URPGAttributeSet* AttributeSet;
};
