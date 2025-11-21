// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/GAS//Task/GSAT_TestTick.h"

UGSAT_TestTick::UGSAT_TestTick(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bTickingTask = true;
}

UGSAT_TestTick* UGSAT_TestTick::CreateTestTickTask(UGameplayAbility* OwningAbility, FName TaskInstanceName)
{
	UGSAT_TestTick* MyObj = NewAbilityTask<UGSAT_TestTick>(OwningAbility, TaskInstanceName);

	return MyObj;
	
}

void UGSAT_TestTick::Activate()
{
	UE_LOG(LogTemp, Warning, TEXT("UGSAT_TestTick::Activate"));
	Super::Activate();
}

void UGSAT_TestTick::TickTask(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("UGSAT_TestTick::TickTask"));
	Super::TickTask(DeltaTime);
}

void UGSAT_TestTick::OnDestroy(bool AbilityEnded)
{
	UE_LOG(LogTemp, Warning, TEXT("UGSAT_TestTick::OnDestroy"));
	Super::OnDestroy(AbilityEnded);
}
