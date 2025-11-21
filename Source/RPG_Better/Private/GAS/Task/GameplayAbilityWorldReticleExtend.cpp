// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Task/GameplayAbilityWorldReticleExtend.h"

#include "Abilities/GameplayAbilityTargetActor.h"

void AGameplayAbilityWorldReticleExtend::Tick(float DeltaTime)
{
	if (GetClass()->HasAnyClassFlags(CLASS_CompiledFromBlueprint) || !GetClass()->HasAnyClassFlags(CLASS_Native))
	{
		// Allow tick if we are not a dedicated server, or we allow this tick on dedicated servers
		if (bAllowReceiveTickEventOnDedicatedServer || !IsRunningDedicatedServer())
		{
			ReceiveTick(DeltaTime);
		}

		// Update any latent actions we have for this actor
		// If this tick is skipped on a frame because we've got a TickInterval, our latent actions will be ticked
		// anyway by UWorld::Tick(). Given that, our latent actions don't need to be passed a larger
		// DeltaSeconds to make up the frames that they missed (because they wouldn't have missed any).
		// So pass in the world's DeltaSeconds value rather than our specific DeltaSeconds value.
		UWorld* MyWorld = GetWorld();
		if (MyWorld)
		{
			FLatentActionManager& LatentActionManager = MyWorld->GetLatentActionManager();
			LatentActionManager.ProcessLatentActions(this, MyWorld->GetDeltaSeconds());
		}
	}

	
	if (TargetingActor->bDebug)
	{
		DebugInfo();
	}
	
}

void AGameplayAbilityWorldReticleExtend::UpdateTargetActor(FHitResult& Hit)
{
	HitResult = Hit;
}

void AGameplayAbilityWorldReticleExtend::SetLocationByImpactPoint()
{
	if (!HitResult.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetLocationByImpactPoint: HitResult is null for TargetActor "));
		return;
	}

	// 3. 安全访问 ImpactPoint
	SetActorLocation(HitResult.ImpactPoint);
}

void AGameplayAbilityWorldReticleExtend::SetLocationByActorLocation()
{
	if (!HitResult.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetLocationByImpactPoint: HitResult is null for TargetActor "));
		return;
	}

	// 3. 安全访问 ImpactPoint
	SetActorLocation(HitResult.GetActor()->GetActorLocation());
}
void AGameplayAbilityWorldReticleExtend::DebugInfo()
{
	UE_LOG(LogTemp, Warning, TEXT("Reticle   StartLocation.SourceActor: %s"), *TargetingActor->StartLocation.SourceActor.GetName())
	UE_LOG(LogTemp, Warning, TEXT("Reticle   StartLocation.SourceComponent: %s"), *TargetingActor->StartLocation.SourceComponent.GetName())
	UE_LOG(LogTemp, Warning, TEXT("Reticle   StartLocation.SourceAbility: %s"), *TargetingActor->StartLocation.SourceAbility.GetName())
}
