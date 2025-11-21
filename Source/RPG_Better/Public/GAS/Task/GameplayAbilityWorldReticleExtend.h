// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Abilities/GameplayAbilityWorldReticle.h"
#include "GameplayAbilityWorldReticleExtend.generated.h"

UCLASS()
class RPG_BETTER_API AGameplayAbilityWorldReticleExtend : public AGameplayAbilityWorldReticle
{
	GENERATED_BODY()

	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void UpdateTargetActor(FHitResult& Hit);
    
	void SetLocationByImpactPoint();
	void SetLocationByActorLocation();

	void DebugInfo();
		

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TargetActor")
	FHitResult HitResult;
};
