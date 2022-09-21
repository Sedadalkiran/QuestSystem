// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"

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

void UQuestComponent::CompleteQuest()
{
	
}

void UQuestComponent::TakeNewQuest()
{    Tasks.Empty();
	
	if(!QuestDataTable.LoadSynchronous())
	{
		return;
	}
	
	FString ContexString=QuestID.ToString();

	FQuestInfo* Row=QuestDataTable.Get()->FindRow<FQuestInfo>(QuestID.GetTagName(),ContexString);
	FGameplayTag::RequestGameplayTag(QuestID.GetTagName());

	if(Row)
	{
		for(auto i:Row->RequiredMissions)
		{
			Tasks.Add(i);
		}
		
	}
}

bool UQuestComponent::CompleteQuest(FGameplayTag QuestID)
{
	
}

bool UQuestComponent::bIsCompletedTasks()
{
	return true;
}

TArray<FQuestRequiredMissions> UQuestComponent::GetRequiredMissions(FGameplayTag QuestID)
{
	if(!QuestDataTable.LoadSynchronous())
	{
		Tasks.Empty();
		return Tasks;
	}

	Tasks.Empty();
	FString ContexString=QuestID.ToString();

	FQuestInfo* Row=QuestDataTable.Get()->FindRow<FQuestInfo>(QuestID.GetTagName(),ContexString);
	FGameplayTag::RequestGameplayTag(QuestID.GetTagName());

	if(Row)
	{
		for(auto i:Row->RequiredMissions)
		{
			Tasks.Add(i);
		}
		
	}
	return Tasks;
}



