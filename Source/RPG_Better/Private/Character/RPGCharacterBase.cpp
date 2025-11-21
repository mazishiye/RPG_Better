// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RPGCharacterBase.h"

#include "GameFramework/GameSession.h"
#include "GAS/RPGAbilitySystemComponent.h"
#include "Player/RPGPlayerController.h"
#include "Player/RPGPlayerState.h"


ARPGCharacterBase::ARPGCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

UAbilitySystemComponent* ARPGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}


void ARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARPGCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (ARPGPlayerState* PS = GetPlayerState<ARPGPlayerState>())
	{
		AbilitySystemComponent = Cast<URPGAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		ARPGPlayerController* PC = Cast<ARPGPlayerController>(GetWorld()->GetFirstPlayerController());
		if(PC)
			PC->AbilitySystemComponent = AbilitySystemComponent;
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	}
}


void ARPGCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (ARPGPlayerState* PS = GetPlayerState<ARPGPlayerState>())
	{
		AbilitySystemComponent = Cast<URPGAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
	}
}
