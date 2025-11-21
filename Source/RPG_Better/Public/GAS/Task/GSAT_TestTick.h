// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"

#include "GSAT_TestTick.generated.h"

/**
 * 
 */
UCLASS()
class RPG_BETTER_API UGSAT_TestTick : public UAbilityTask
{
	GENERATED_BODY()
public:
	UGSAT_TestTick(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UGSAT_TestTick* CreateTestTickTask(UGameplayAbility* OwningAbility, FName TaskInstanceName);
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool AbilityEnded) override;
};
