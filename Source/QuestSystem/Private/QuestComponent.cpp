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
	if(!CurrentQuests.IsValidIndex(CurrentQuestIndex))
	{
		return FGameplayTag::EmptyTag;
	}
	return CurrentQuests[CurrentQuestIndex];
}

void UQuestComponent::GetCurrentQuestTasks(TArray<FTaskState>& OutTasks)
{
	TArray<FTaskState> TasksStates;
	GetRequiredTasksForQuest(GetCurrentQuestID(),TasksStates);
	OutTasks=TasksStates;
}


void UQuestComponent::TakeNewQuest(FGameplayTag QuestID)
{    
	if(!QuestDataTable.LoadSynchronous())
	{
		return;
	}
	FString ContexString=QuestID.ToString();

	FQuestInfo* Row=QuestDataTable.Get()->FindRow<FQuestInfo>(QuestID.GetTagName(),ContexString);
	
	if(Row)
	{
		if(IsRequiredQuestsCompletedForQuest(QuestID))
		{
			//FGameplayTag::RequestGameplayTag(QuestID.GetTagName());
			CurrentQuestIndex=CurrentQuests.AddUnique(Row->QuestID);
		
			TArray<FTaskState>& RequiredTasks=CurrentQuestTasks.FindOrAdd(Row->QuestID);
			RequiredTasks.Empty();
			for(const auto& RequiredTask:Row->RequiredTasks)
			{
				FTaskState State;
				State.TaskData=RequiredTask;
				RequiredTasks.Add(State);
			}
		}
		
		
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
	bool bCompletedAllQuestTasks=false;
	TArray<FTaskState> TaskStates;
	GetRequiredTasksForQuest(QuestID,TaskStates);
	for (auto TaskState : TaskStates)
	{
		bCompletedAllQuestTasks=TaskState.bIsCompleted;
		if (!bCompletedAllQuestTasks)
		{
			return bCompletedAllQuestTasks;
		}
	}
	return bCompletedAllQuestTasks;
}

int32 UQuestComponent::GetCurrentTaskRequirements(FGameplayTag TaskID)
{
	if(!QuestDataTable.LoadSynchronous())
    	{
    		return -1;
    	}
	FString ContexString=GetCurrentQuestID().ToString();


	FQuestInfo* Row = QuestDataTable.Get()->FindRow<FQuestInfo>(GetCurrentQuestID().GetTagName(),ContexString);
	
	if(Row)
	{
		TArray<FTaskData>& Tasks = Row->RequiredTasks;
		for (const auto& Task : Tasks)
		{
			if(Task.TaskID == TaskID)
			{
				return Task.RequiredAmount;
			}
		}
	}
	return -1;
}


bool UQuestComponent::GetRequiredTasksForQuest(FGameplayTag QuestID, TArray<FTaskState>& OutTasks)
{
	TArray<FTaskState>* TaskStates = CurrentQuestTasks.Find(QuestID);
    if(TaskStates)
    {
     	OutTasks = *TaskStates;
    	return true;
    }
	return false;
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
	bool bIsCompletedRequiredQuests=false;
	
	if(!QuestDataTable.LoadSynchronous())
	{
		return false;
	}
	FString ContexString=QuestID.ToString();

	FQuestInfo* Row=QuestDataTable.Get()->FindRow<FQuestInfo>(QuestID.GetTagName(),ContexString);
	
	if(Row)
	{
		if(Row->RequiredQuests.IsEmpty())
		{
			bIsCompletedRequiredQuests=true;
		}
		else
		{
			TArray<FGameplayTag> RequiredQuest=Row->RequiredQuests;
			for (auto Quest : RequiredQuest)
			{
				bIsCompletedRequiredQuests=FinishedQuests.Contains(Quest);
				if(!bIsCompletedRequiredQuests)
				{
					return bIsCompletedRequiredQuests;
				}
			}
		}
	
	}
	return bIsCompletedRequiredQuests;
	
}



