// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilityWorldReticleExtend.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GameplayAbilityTargetActor_UnderCursorTrace.generated.h"

UCLASS()
class RPG_BETTER_API AGameplayAbilityTargetActor_UnderCursorTrace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AGameplayAbilityTargetActor_UnderCursorTrace(const FObjectInitializer& ObjectInitializer);
	
	bool  UnderCursorTrace(FHitResult& OutHitResult);
	
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	
	virtual void ConfirmTargeting() override;
	
	virtual void CancelTargeting() override;

	virtual void Destroyed() override;
	
	virtual FGameplayAbilityTargetDataHandle MakeTargetData(const FHitResult& HitResult) const;
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void DebugInfo();

private:
	
	TWeakObjectPtr<AGameplayAbilityWorldReticleExtend> ReticleActor;
};
