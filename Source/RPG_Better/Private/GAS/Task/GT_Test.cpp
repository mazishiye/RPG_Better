// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Task/GT_Test.h"

UGT_Test* UGT_Test::TaskTest(TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner, FName TaskInstanceName,
							   bool bIstasktick, uint8 Priority, ETaskResourceOverlapPolicy OverlapPolicy,
							   uint8 ResourceID)

{
	if (TaskOwner.GetInterface() == nullptr)
	{
		return nullptr;
	}


	UGT_Test* MyTask = nullptr;

	MyTask = NewTask<UGT_Test>(TaskOwner);
	if (MyTask)
	{
		MyTask->InstanceName = TaskInstanceName;
		MyTask->bTickingTask = bIstasktick;
		MyTask->Priority = Priority;
		MyTask->ResourceOverlapPolicy = OverlapPolicy;
		MyTask->ClaimedResources.AddID(ResourceID);
	}


	return MyTask;
}

void UGT_Test::Pause()
{
	UE_LOG(LogTemp, Log, TEXT("%s::Pause"), *GetInstanceName().ToString());
	bTickingTask = false;
	Super::Pause();
}

void UGT_Test::Resume()
{
	UE_LOG(LogTemp, Log, TEXT("%s::Resume"), *GetInstanceName().ToString());
	bTickingTask = true;
	Super::Resume();
}

void UGT_Test::AddResourceID(uint8 ResourceID)
{
	ClaimedResources.AddID(ResourceID);
}

void UGT_Test::Activate()
{
	UE_LOG(LogTemp, Log, TEXT("%s::Activate"), *GetInstanceName().ToString());
	Super::Activate();
}

void UGT_Test::TickTask(float DeltaTime)
{
	UE_LOG(LogTemp, Log, TEXT("%s::TickTask"), *GetInstanceName().ToString());
	Super::TickTask(DeltaTime);
}

void UGT_Test::ExternalConfirm(bool bEndTask)
{
	UE_LOG(LogTemp, Log, TEXT("%s::ExternalConfirm"), *GetInstanceName().ToString());
	Super::ExternalConfirm(bEndTask);
}

void UGT_Test::ExternalCancel()
{
	UE_LOG(LogTemp, Log, TEXT("%s::ExternalCancel"), *GetInstanceName().ToString());
	Super::ExternalCancel();
}

void UGT_Test::OnDestroy(bool bInOwnerFinished)
{
	UE_LOG(LogTemp, Log, TEXT("%s::OnDestroy"), *GetInstanceName().ToString());
	Super::OnDestroy(bInOwnerFinished);
}