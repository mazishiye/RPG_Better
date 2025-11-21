// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Abilities/GameplayAbilityTargetActor_Trace.h"
#include "GameplayAbilityTargetActor_TopDownTrace.generated.h"

UCLASS()
class RPG_BETTER_API AGameplayAbilityTargetActor_TopDownTrace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	AGameplayAbilityTargetActor_TopDownTrace();

	/** 追踪高度（从起始点向下的垂直距离，用于处理俯视角的高度差异） */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "TopDown Trace")
	float TraceHeight;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "TopDown Trace")
	float MaxRange;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "TopDown Trace")
	FCollisionProfileName TraceProfile;
	/** 是否显示调试追踪线 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "TopDown Trace|Debug")
	bool bShowDebugTrace;

	/** 调试追踪线的颜色 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "TopDown Trace|Debug")
	FColor DebugTraceColor;

	/** 调试追踪线的持续时间 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "TopDown Trace|Debug")
	float DebugTraceDuration;

	// ==================== 蓝图可调用方法 ====================

	/** 设置追踪方向（俯视角通常使用世界空间方向） */
	UFUNCTION(BlueprintCallable, Category = "TopDown Trace")
	void SetTraceDirection(const FVector& Direction);

	/** 设置追踪范围 */
	UFUNCTION(BlueprintCallable, Category = "TopDown Trace")
	void SetTraceRange(float Range);

	/** 立即执行一次追踪并返回结果 */
	UFUNCTION(BlueprintCallable, Category = "TopDown Trace")
	FHitResult PerformImmediateTrace();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "TopDown Trace")
	void UpdateIndicator(const FHitResult& HitResult);

	// ==================== 重写父类方法 ====================

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	//virtual void ConfirmTargetingAndContinue() override;
	virtual void ConfirmTargeting() override;
	virtual void CancelTargeting() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	// ==================== 保护成员变量 ====================

	/** 当前的追踪方向 */
	FVector CurrentTraceDirection;

	/** 是否使用自定义追踪方向 */
	bool bUseCustomDirection;

	/** 准星Actor实例 */
	UPROPERTY()
	TObjectPtr<AGameplayAbilityWorldReticle> ReticleActor;

	// ==================== 保护方法 ====================

	/** 执行俯视角追踪逻辑 */
	virtual FHitResult PerformTopDownTrace();
	static  void SweepWithFilter(FHitResult& OutHitResult, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, const FQuat& Rotation, const FCollisionShape CollisionShape, FName ProfileName, const FCollisionQueryParams Params);

	/** 创建目标数据 */
	virtual FGameplayAbilityTargetDataHandle MakeTargetData(const FHitResult& HitResult) const;
	FGameplayAbilityTargetDataHandle MakeTargetsData(const TArray<FHitResult>& HitResults) const;

	/** 更新准星位置 */
	virtual void UpdateReticlePosition(const FHitResult& HitResult);

	/** 获取追踪起点 */
	virtual FVector GetTraceStartLocation() const;

	/** 获取追踪终点 */
	virtual FVector GetTraceEndLocation() const;

	/** 绘制调试信息 */
	virtual void DrawDebugTrace(const FVector& Start, const FVector& End, const FHitResult& HitResult) const;

	/** 从鼠标/屏幕位置获取世界空间方向（俯视角专用） */
	virtual FVector GetTraceDirectionFromMouse() const;

	/** 从游戏手柄输入获取世界空间方向（俯视角专用） */
	virtual FVector GetTraceDirectionFromGamepad() const;

	/** 生成或获取准星Actor */
	virtual AGameplayAbilityWorldReticle* GetOrCreateReticleActor();
};
