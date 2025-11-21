// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Task/GameplayAbilityTargetActor_TopDownTrace.h"

#include "Abilities/GameplayAbility.h"
#include "GameFramework/GameSession.h"


AGameplayAbilityTargetActor_TopDownTrace::AGameplayAbilityTargetActor_TopDownTrace()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;

	// 默认配置
	MaxRange = 3000.0f;
	TraceHeight = 500.0f; // 俯视角通常需要一定高度来捕捉地面目标
	bShowDebugTrace = false;
	DebugTraceColor = FColor::Green;
	DebugTraceDuration = 2.0f;
	bUseCustomDirection = false;
	CurrentTraceDirection = FVector::ForwardVector;

	// 俯视角通常在服务器上生成目标数据
	ShouldProduceTargetDataOnServer = true;
	bDestroyOnConfirmation = true;
}
void AGameplayAbilityTargetActor_TopDownTrace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
    
	if (PrimaryPC && OwningAbility)
	{
		// 初始化默认追踪方向
		if (!bUseCustomDirection)
		{
			CurrentTraceDirection = GetActorForwardVector();
		}
        
		// 生成准星（如果配置了准星类）
		GetOrCreateReticleActor();
	}
}
// void AGameplayAbilityTargetActor_TopDownTrace::ConfirmTargetingAndContinue()
// {
// 	// 执行最终追踪
// 	FHitResult HitResult = PerformTopDownTrace();
//     
// 	if (ShouldProduceTargetData())
// 	{
// 		// 创建目标数据并广播
// 		FGameplayAbilityTargetDataHandle Handle = MakeTargetData(HitResult);
// 		UE_LOG(LogTemp, Log, TEXT("ConfirmTargetingAndContinue"))
// 		TargetDataReadyDelegate.Broadcast(Handle);
// 	}
//     
// 	// 更新准星位置
// 	UpdateReticlePosition(HitResult);
// }
void AGameplayAbilityTargetActor_TopDownTrace::ConfirmTargeting()
{
	FHitResult HitResult = PerformTopDownTrace();
	FVector location = HitResult.Location;
	FCollisionShape FilterShape;
	FilterShape.SetSphere(200.0f);

	TArray<FHitResult> HitResults;
	GetWorld()->SweepMultiByProfile(HitResults, location, location, FQuat(), TraceProfile.Name, FilterShape);
	
	if (ShouldProduceTargetData())
	{
		FGameplayAbilityTargetDataHandle Handle = MakeTargetsData(HitResults);
		TargetDataReadyDelegate.Broadcast(Handle);
	}
    
	Super::ConfirmTargeting();
}
void AGameplayAbilityTargetActor_TopDownTrace::SweepWithFilter(FHitResult& OutHitResult, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, const FQuat& Rotation, const FCollisionShape CollisionShape, FName ProfileName, const FCollisionQueryParams Params)
{
	check(World);

	TArray<FHitResult> HitResults;
	World->SweepMultiByProfile(HitResults, Start, End, Rotation, ProfileName, CollisionShape, Params);
	
	//DrawSphere(World, Start, 200.0f, 50.0f, FColor::Red,false,5.f)
	OutHitResult.TraceStart = Start;
	OutHitResult.TraceEnd = End;

	for (int32 HitIdx = 0; HitIdx < HitResults.Num(); ++HitIdx)
	{
		const FHitResult& Hit = HitResults[HitIdx];

		if (!Hit.HitObjectHandle.IsValid() || FilterHandle.FilterPassesForActor(Hit.HitObjectHandle.FetchActor()))
		{
			OutHitResult = Hit;
			OutHitResult.bBlockingHit = true; // treat it as a blocking hit
			return;
		}
	}
}

void AGameplayAbilityTargetActor_TopDownTrace::CancelTargeting()
{
	// 清理准星
	if (ReticleActor)
	{
		ReticleActor->Destroy();
		ReticleActor = nullptr;
	}
    
	Super::CancelTargeting();
}
void AGameplayAbilityTargetActor_TopDownTrace::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
    
	//if (SourceActor && SourceActor->GetLocalRole() != ENetRole::ROLE_SimulatedProxy)	
	if (true)	
	{
		// 持续更新追踪和准星位置
		FHitResult HitResult = PerformTopDownTrace();
		UpdateReticlePosition(HitResult);
		UpdateIndicator(HitResult);
        
		// 绘制调试信息
		if (bShowDebugTrace)
		{
			DrawDebugTrace(GetTraceStartLocation(), GetTraceEndLocation(), HitResult);
		}
	}
}
FHitResult AGameplayAbilityTargetActor_TopDownTrace::PerformTopDownTrace()
{
	if (!OwningAbility) // Server and launching client only
	{
		return FHitResult();
	}
	APlayerController* PC = OwningAbility->GetCurrentActorInfo()->PlayerController.Get();
	check(PC);
    FVector TraceStart = GetTraceStartLocation();
    FVector TraceEnd = GetTraceEndLocation();
    
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.bTraceComplex = true;
    QueryParams.bReturnPhysicalMaterial = false;
    
    if (SourceActor)
    {
        QueryParams.AddIgnoredActor(SourceActor);
    }
	PC->GetHitResultUnderCursor(ECC_Visibility, false,HitResult);
    // 执行追踪
    // GetWorld()->LineTraceSingleByProfile(
    //     HitResult,
    //     TraceStart,
    //     TraceEnd,
    //     TraceProfile.Name,
    //     QueryParams
    // );
	

	
    
    return HitResult;
}
FGameplayAbilityTargetDataHandle AGameplayAbilityTargetActor_TopDownTrace::MakeTargetData(const FHitResult& HitResult) const
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

FGameplayAbilityTargetDataHandle AGameplayAbilityTargetActor_TopDownTrace::MakeTargetsData(
	const TArray<FHitResult>& HitResults) const
{
	FGameplayAbilityTargetDataHandle Handle;
    
	if (!HitResults.IsEmpty())
	{
		// 创建基于命中结果的目标数据
		for(const FHitResult& HitResult : HitResults)
		{
			if (HitResult.bBlockingHit)
			{
				// 命中时：创建单目标命中数据
				FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit();
				TargetData->HitResult = HitResult;
				Handle.Add(TargetData); // 每个对象只添加一次
				//print hitresult'name
				UE_LOG(LogTemp, Log, TEXT("MakeTargetData: %s"), *HitResult.GetActor()->GetName());
			}
			else
			{
				// 未命中单个目标时：创建位置数据（和MakeTargetData逻辑保持一致）
				FGameplayAbilityTargetData_LocationInfo* LocationData = new FGameplayAbilityTargetData_LocationInfo();
				LocationData->TargetLocation.LiteralTransform = FTransform(HitResult.TraceEnd);
				Handle.Add(LocationData);
			}
		}	
	}
	else
	{
		// 关键修复2：无任何命中结果时，不创建无效对象（Handle保持为空）
		UE_LOG(LogTemp, Warning, TEXT("MakeTargetsData: No hit results"));
	}
	
    
	return Handle;
}

void AGameplayAbilityTargetActor_TopDownTrace::UpdateReticlePosition(const FHitResult& HitResult)
{
	AGameplayAbilityWorldReticle* Reticle = GetOrCreateReticleActor();
	if (Reticle)
	{
		FVector ReticleLocation = HitResult.bBlockingHit ? HitResult.Location : HitResult.TraceEnd;
        
		// 俯视角通常需要将准星放在地面上方一点的位置
		ReticleLocation.Z += 50.0f; 
        
		Reticle->SetActorLocation(ReticleLocation);
	}
}
FVector AGameplayAbilityTargetActor_TopDownTrace::GetTraceStartLocation() const
{
	if (StartLocation.LocationType == EGameplayAbilityTargetingLocationType::LiteralTransform)
	{
		return StartLocation.LiteralTransform.GetLocation();
	}
	else if (StartLocation.SourceActor && StartLocation.SourceComponent)
	{
		return StartLocation.SourceComponent->GetComponentLocation();
	}
	else if (SourceActor)
	{
		return SourceActor->GetActorLocation();
	}
    
	return GetActorLocation();
}
FVector AGameplayAbilityTargetActor_TopDownTrace::GetTraceEndLocation() const
{
	FVector Start = GetTraceStartLocation();
    
	// 俯视角追踪：在X-Y平面计算方向，忽略Z轴
	FVector Direction = CurrentTraceDirection;
	Direction.Z = 0.0f;  // 确保在水平面
	Direction.Normalize();
    
	FVector End = Start + (Direction * MaxRange);
    
	// 俯视角考虑追踪高度（从高处向下追踪）
	End.Z -= TraceHeight;
    
	return End;
}
void AGameplayAbilityTargetActor_TopDownTrace::SetTraceDirection(const FVector& Direction)
{
	CurrentTraceDirection = Direction;
	bUseCustomDirection = true;
}
void AGameplayAbilityTargetActor_TopDownTrace::SetTraceRange(float Range)
{
	MaxRange = Range;
}
FHitResult AGameplayAbilityTargetActor_TopDownTrace::PerformImmediateTrace()
{
	return PerformTopDownTrace();
}
void AGameplayAbilityTargetActor_TopDownTrace::DrawDebugTrace(const FVector& Start, const FVector& End, const FHitResult& HitResult) const
{
	//get creame location
	FVector CameraLocation;
	FRotator CameraRotation;
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);
	
	if (bShowDebugTrace && GetWorld())
	{
		::DrawDebugLine(
			GetWorld(),
			OwningAbility->GetCurrentActorInfo()->OwnerActor->GetActorLocation(),
			HitResult.bBlockingHit ? HitResult.Location : End,
			DebugTraceColor,
			false,
			DebugTraceDuration,
			0,
			2.0f
		);
		DrawDebugSphere(GetWorld(),HitResult.bBlockingHit ? HitResult.Location : End, 200.0f, 16, FColor::Red,false,DebugTraceDuration);
		if (HitResult.bBlockingHit)
		{
			::DrawDebugPoint(
				GetWorld(),
				HitResult.Location,
				10.0f,
				FColor::Red,
				false,
				DebugTraceDuration
			);
		}
	}
}
FVector AGameplayAbilityTargetActor_TopDownTrace::GetTraceDirectionFromMouse() const
{
	if (!PrimaryPC) return FVector::ForwardVector;
    
	FVector WorldLocation, WorldDirection;
	if (PrimaryPC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		// 俯视角：将鼠标位置转换为水平方向
		FVector TraceStart = GetTraceStartLocation();
        
		// 创建从追踪起点到鼠标方向的平面追踪
		FVector MouseDirection = (WorldLocation - TraceStart);
		MouseDirection.Z = 0;  // 忽略Z轴
		MouseDirection.Normalize();
        
		return MouseDirection;
	}
    
	return FVector::ForwardVector;
}
FVector AGameplayAbilityTargetActor_TopDownTrace::GetTraceDirectionFromGamepad() const
{
	if (!PrimaryPC) return FVector::ForwardVector;
    
	// 获取游戏手柄右摇杆输入
	FVector InputDirection = FVector::ZeroVector;
	// 这里需要根据您的输入系统实现获取游戏手柄输入
    
	if (InputDirection.SizeSquared() > 0.1f)
	{
		InputDirection.Z = 0;  // 俯视角忽略Z轴输入
		InputDirection.Normalize();
		return InputDirection;
	}
    
	return CurrentTraceDirection;
}
AGameplayAbilityWorldReticle* AGameplayAbilityTargetActor_TopDownTrace::GetOrCreateReticleActor()
{
	if (!ReticleActor && ReticleClass && GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
        
		FVector SpawnLocation = GetTraceStartLocation();
		ReticleActor = GetWorld()->SpawnActor<AGameplayAbilityWorldReticle>(ReticleClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	}
    
	return ReticleActor;
}
void AGameplayAbilityTargetActor_TopDownTrace::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ReticleActor)
	{
		ReticleActor->Destroy();
		ReticleActor = nullptr;
	}
    
	Super::EndPlay(EndPlayReason);
}