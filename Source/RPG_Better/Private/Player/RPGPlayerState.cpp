// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RPGPlayerState.h"

#include "GAS/RPGAbilitySystemComponent.h"
#include "GAS/RPGAttributeSet.h"

ARPGPlayerState::ARPGPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<URPGAttributeSet>(TEXT("AttributeSet"));

	AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet);

	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* ARPGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

URPGAttributeSet* ARPGPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

void ARPGPlayerState::BeginPlay()
{
	Super::BeginPlay();
	if(InitAttribute())
		UE_LOG(LogTemp,Warning,TEXT("InitAttribute successful"))
}

bool ARPGPlayerState::InitAttribute()
{
	if(!AttributeEffect)
		return false;

	const FGameplayEffectSpecHandle Spec =  AbilitySystemComponent->MakeOutgoingSpec(AttributeEffect,0,AbilitySystemComponent->MakeEffectContext());
	const FActiveGameplayEffectHandle  Handle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data);
	if(Handle.IsValid())
		return true;
	return false;
		
}
