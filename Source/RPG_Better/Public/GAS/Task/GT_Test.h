// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTask.h"
#include "GT_Test.generated.h"

/**
 * 
 */
UCLASS()
class RPG_BETTER_API UGT_Test : public UGameplayTask
{
	GENERATED_BODY()
public:	
	UFUNCTION(BlueprintCallable, Category = "GameplayTasks", meta = (DisplayName="Task Test", DefaultToSelf = "TaskOwner", BlueprintInternalUseOnly = "TRUE"))
	static UGT_Test* TaskTest(TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner,FName TaskInstanceName, bool bIstasktick,uint8 Priority,ETaskResourceOverlapPolicy OverlapPolicy, uint8 ResourceID);

	UFUNCTION(BlueprintCallable)
	virtual void Pause() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void Resume() override;

	UFUNCTION(BlueprintCallable)
	void AddResourceID(uint8 ResourceID);

	
	virtual void Activate() override;
	// 任务每帧更新
	virtual void TickTask(float DeltaTime) override;
	// 任务被外部确认（如玩家输入确认）
	virtual void ExternalConfirm(bool bEndTask) override;
	// 任务被取消
	virtual void ExternalCancel() override;
	// 任务销毁清理
	virtual void OnDestroy(bool bInOwnerFinished) override;
	
};
