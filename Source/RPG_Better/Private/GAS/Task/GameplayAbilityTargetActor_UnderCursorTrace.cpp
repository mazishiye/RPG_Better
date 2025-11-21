// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Task/GameplayAbilityTargetActor_UnderCursorTrace.h"


// Sets default values
AGameplayAbilityTargetActor_UnderCursorTrace::AGameplayAbilityTargetActor_UnderCursorTrace(
	const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}


bool AGameplayAbilityTargetActor_UnderCursorTrace::UnderCursorTrace(FHitResult& OutHitResult)
{
	if (!PrimaryPC)
	{
		UE_LOG(LogTemp, Error,
		       TEXT("AGameplayAbilityTargetActor_UnderCursorTrace::UnderCursorTrace: PrimaryPC is nullptr"))
		return false;
	}
	return PrimaryPC->GetHitResultUnderCursor(ECC_Visibility, false, OutHitResult);
}


void AGameplayAbilityTargetActor_UnderCursorTrace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	if (ReticleClass)
	{
		AGameplayAbilityWorldReticleExtend* SpawnedReticleActor = GetWorld()->SpawnActor<
			AGameplayAbilityWorldReticleExtend>(ReticleClass, GetActorLocation(), GetActorRotation());
		if (SpawnedReticleActor)
		{
			SpawnedReticleActor->InitializeReticle(this, PrimaryPC, ReticleParams);
			ReticleActor = SpawnedReticleActor;
			if (!ShouldProduceTargetDataOnServer)
			{
				SpawnedReticleActor->SetReplicates(false);
			}
		}
	}
}


void AGameplayAbilityTargetActor_UnderCursorTrace::ConfirmTargeting()
{
	FHitResult HitResult;
	if (!UnderCursorTrace(HitResult))
	{
		UE_LOG(LogTemp, Error,
		       TEXT("AGameplayAbilityTargetActor_UnderCursorTrace::ConfirmTargeting: UnderCursorTrace failed"))
	}
	//use filter
	FGameplayAbilityTargetDataHandle Handle = MakeTargetData(HitResult);
	TargetDataReadyDelegate.Broadcast(Handle);

	Super::ConfirmTargeting();
}

void AGameplayAbilityTargetActor_UnderCursorTrace::CancelTargeting()
{
	Super::CancelTargeting();
}

void AGameplayAbilityTargetActor_UnderCursorTrace::Destroyed()
{
	if (ReticleActor.Get())
	{
		ReticleActor->Destroy();
	}
	Super::Destroyed();
}

FGameplayAbilityTargetDataHandle AGameplayAbilityTargetActor_UnderCursorTrace::MakeTargetData(
	const FHitResult& HitResult) const
{
	FGameplayAbilityTargetDataHandle Handle;

	if (HitResult.bBlockingHit)
	{
		// 创建基于命中结果的目标数据
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit();
		TargetData->HitResult = HitResult;
		Handle.Add(TargetData);
	}
	else
	{
		// 如果没有命中，创建基于位置的目标数据
		FGameplayAbilityTargetData_LocationInfo* LocationData = new FGameplayAbilityTargetData_LocationInfo();
		LocationData->TargetLocation.LiteralTransform = FTransform(HitResult.TraceEnd);
		Handle.Add(LocationData);
	}

	return Handle;
}

// Called every frame
void AGameplayAbilityTargetActor_UnderCursorTrace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FHitResult HitResult;
	UnderCursorTrace(HitResult);
	ReticleActor->UpdateTargetActor(HitResult);
	if (bDebug)
		DebugInfo();
}

void AGameplayAbilityTargetActor_UnderCursorTrace::DebugInfo()
{
	UE_LOG(LogTemp, Warning, TEXT("StartLocation.SourceActor: %s"), *StartLocation.SourceActor.GetName())
	UE_LOG(LogTemp, Warning, TEXT("StartLocation.SourceComponent: %s"), *StartLocation.SourceComponent.GetName())
	UE_LOG(LogTemp, Warning, TEXT("StartLocation.SourceAbility: %s"), *StartLocation.SourceAbility.GetName())
}
