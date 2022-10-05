// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"

#include "IPropertyTable.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FGameplayTag UQuestComponent::GetCurrentQuestID() const
{
	if (!CurrentQuests.IsValidIndex(CurrentQuestIndex))
	{
		return FGameplayTag::EmptyTag;
	}
	return CurrentQuests[CurrentQuestIndex];
}

void UQuestComponent::GetCurrentQuestTasks(TArray<FTaskState>& OutTasks)
{
	OutTasks = GetRequiredTasksForQuest(GetCurrentQuestID());
}


void UQuestComponent::TakeNewQuest(FGameplayTag QuestID)
{
	if (bHasActiveQuest)
	{
		UE_LOG(LogTemp,Warning,TEXT("%s questi alınamadı aktif quest : %s"),*QuestID.ToString(),*GetCurrentQuestID().ToString());
		return;
	}

	if(FinishedQuests.Contains(QuestID))
	{   UE_LOG(LogTemp,Warning,TEXT("%s questi alınamadı, bitmiş questler içerisinde yer alıyor"),*QuestID.ToString(),*GetCurrentQuestID().ToString());
		return;
	}
	
	if (!QuestDataTable.LoadSynchronous())
	{
		UE_LOG(LogTemp,Warning,TEXT("%s questi alınamadı data table yüklenemedi"),*QuestID.ToString());
		return;
	}
	FString ContexString = QuestID.ToString();

	FQuestInfo* Row = QuestDataTable.Get()->FindRow<FQuestInfo>(QuestID.GetTagName(), ContexString);

	if (Row)
	{
		if (IsRequiredQuestsCompletedForQuest(QuestID))
		{
			//FGameplayTag::RequestGameplayTag(QuestID.GetTagName());
			CurrentQuestIndex = CurrentQuests.AddUnique(Row->QuestID);

			TArray<FTaskState>& RequiredTasks = CurrentQuestTasks.FindOrAdd(Row->QuestID);
			RequiredTasks.Empty();
			for (const auto& RequiredTask : Row->RequiredTasks)
			{
				FTaskState State;
				State.TaskData = RequiredTask;
				RequiredTasks.Add(State);
			}
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("%s questi alınamadı gerekli questler tamamlanmadı"),*QuestID.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("%s questi alınamadı row bulunamadı"),*QuestID.ToString());
	}
}

void UQuestComponent::UpdateQuestList()
{
}

bool UQuestComponent::CompleteTaskForCurrentQuest(FGameplayTag TaskID)
{
	return SetTaskCompletedForQuest(GetCurrentQuestID(), TaskID, true);;
}

bool UQuestComponent::CompleteQuest(FGameplayTag QuestID)
{
	
	TArray<FTaskState> TaskStates = GetRequiredTasksForQuest(QuestID);
	for (const auto& TaskState : TaskStates)
	{
		if (!TaskState.bIsCompleted)
		{
			UE_LOG(LogTemp,Warning,TEXT("%s questi için gereken %s taskı tamamlanmadı"),*QuestID.ToString(),*TaskState.TaskData.TaskID.ToString());
			return false;
		}
	}
	
	
	if(CurrentQuests.Remove(QuestID)>0)
	{
		UE_LOG(LogTemp,Warning,TEXT("%s questi tamamlandı sebebiii bilinmiyor"),*QuestID.ToString());
		bHasActiveQuest=false;
		FinishedQuests.AddUnique(QuestID);
		return true;
	}
	UE_LOG(LogTemp,Warning,TEXT("%s questi için şuanki questler arasında değilll quest tamamlanamadı"),*QuestID.ToString());
	return false;
}

int32 UQuestComponent::GetCurrentTaskRequirements(FGameplayTag TaskID)
{
	if (!QuestDataTable.LoadSynchronous())
	{
		return -1;
	}
	FString ContexString = GetCurrentQuestID().ToString();


	FQuestInfo* Row = QuestDataTable.Get()->FindRow<FQuestInfo>(GetCurrentQuestID().GetTagName(), ContexString);

	if (Row)
	{
		TArray<FTaskData>& Tasks = Row->RequiredTasks;
		for (const auto& Task : Tasks)
		{
			if (Task.TaskID == TaskID)
			{
				return Task.RequiredAmount;
			}
		}
	}
	return -1;
}


TArray<FTaskState> UQuestComponent::GetRequiredTasksForQuest(FGameplayTag QuestID)
{
	TArray<FTaskState>* TaskStates = CurrentQuestTasks.Find(QuestID);
	if (TaskStates)
	{
		return  *TaskStates;
		
	}
	return TArray<FTaskState>{};
}

bool UQuestComponent::SetTaskCompletedForQuest(FGameplayTag QuestID, FGameplayTag TaskID, bool bCompleted)
{
	TArray<FTaskState>* TaskStates = CurrentQuestTasks.Find(QuestID);
	if (TaskStates)
	{
		for (FTaskState& TaskState : *TaskStates)
		{
			if (TaskState.TaskData.TaskID == TaskID)
			{
				TaskState.bIsCompleted = bCompleted;
				return true;
			}
		}
	}

	return false;
}

bool UQuestComponent::IsRequiredQuestsCompletedForQuest(FGameplayTag QuestID)
{
	

	if (!QuestDataTable.LoadSynchronous())
	{
		return false;
	}
	FString ContexString = QuestID.ToString();

	FQuestInfo* Row = QuestDataTable.Get()->FindRow<FQuestInfo>(QuestID.GetTagName(), ContexString);

	if (Row)
	{
		if (Row->RequiredQuests.IsEmpty())
		{
			return true;
		}
		else
		{
			TArray<FGameplayTag> RequiredQuest = Row->RequiredQuests;
			bool bIsCompletedRequiredQuests = false;
			for (auto Quest : RequiredQuest)
			{
				bIsCompletedRequiredQuests = FinishedQuests.Contains(Quest);
				if (!bIsCompletedRequiredQuests)
				{
					return false;
				}
			}
		}
	}
	return true;
}
