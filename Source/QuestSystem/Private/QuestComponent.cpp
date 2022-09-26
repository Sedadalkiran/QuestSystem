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

TArray<FTaskState> UQuestComponent::GetCurrentQuestTasks() const
{
	return GetRequiredTasksForQuest(GetCurrentQuestID());
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

void UQuestComponent::UpdateQuestList()
{
	
}

bool UQuestComponent::CompleteQuest(FGameplayTag QuestID)
{
	return true;
}

bool UQuestComponent::bIsCompletedTasks()
{
	return true;
}



TArray<FTaskState>  UQuestComponent::GetRequiredTasksForQuest(FGameplayTag QuestID) const
{
	const TArray<FTaskState>* CurrentTasks= CurrentQuestTasks.Find(QuestID);
	
	return CurrentTasks ? *CurrentTasks : TArray<FTaskState>{};
}



