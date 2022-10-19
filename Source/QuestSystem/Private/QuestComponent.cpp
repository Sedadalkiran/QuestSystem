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

bool UQuestComponent::CanTakeQuest(FGameplayTag QuestID)
{
	if (CurrentQuest.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s questi alınamadı aktif quest : %s"), *QuestID.ToString(),
			   *CurrentQuest.ToString());
		return false;
	}

	if (FinishedQuests.Contains(QuestID))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s questi alınamadı, bitmiş questler içerisinde yer alıyor"),
			   *QuestID.ToString(), *CurrentQuest.ToString());
		return false;
	}
	if (!IsRequiredQuestsCompletedForQuest(QuestID))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s questi alınamadı gerekli questler tamamlanmadı"), *QuestID.ToString());
		return false;
	}
	return true;
}

bool UQuestComponent::TakeNewQuest(FGameplayTag QuestID)
{
	if(!CanTakeQuest(QuestID))
	{
		return false;
	}

	auto* QuestDataTablePtr=QuestDataTable.LoadSynchronous();
	check(QuestDataTablePtr);
	
	FString ContexString = QuestID.ToString();

	FQuestInfo* Row = QuestDataTablePtr->FindRow<FQuestInfo>(QuestID.GetTagName(), ContexString);

	if (Row)
	{
			//FGameplayTag::RequestGameplayTag(QuestID.GetTagName());
			CurrentQuest = QuestID;

			CurrentQuestTasks = Row->RequiredTasks;
			RefreshQuest();
		return true;
	}
	
		UE_LOG(LogTemp, Error, TEXT("%s questi alınamadı row bulunamadı"), *QuestID.ToString());
	
	return false;
}

EQuestRefreshState UQuestComponent::RefreshQuest()
{
	//If Current Quest does not exist
	if (!CurrentQuest.IsValid())
	{
		return EQuestRefreshState::InvalidCurrentQuest;
	}

	//If Current Quest is alreadyfinish

	if (CompleteQuest())
	{
		return EQuestRefreshState::QuestCompleted;
	}

	//Proceed To Next Task
	for (int i = 0; i < CurrentQuestTasks.Num(); i++)
	{
		if (!CurrentQuestTasks[i].IsAllRequirementsCompleted())
		{
			if (ActiveTaskIndex != i)
			{
				ActiveTaskIndex = i;
				return EQuestRefreshState::NextTaskStarted;
			}
		}
	}


	return EQuestRefreshState::NoUpdate;
}


void UQuestComponent::UpdateQuestList()
{
}


bool UQuestComponent::CompleteQuest()
{
	for (const auto& TaskState : CurrentQuestTasks)
	{
		if (!TaskState.IsAllRequirementsCompleted())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s questi için gereken %s taskı tamamlanmadı"), *CurrentQuest.ToString(),
			       *TaskState.TaskID.ToString());
			return false;
		}
	}

	if (CurrentQuest.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s questi tamamlandı sebebiii bilinmiyor"), *CurrentQuest.ToString());

		FinishedQuests.AddUnique(CurrentQuest);
		ActiveTaskIndex = -1;
		CurrentQuest = FGameplayTag::EmptyTag;
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("%s questi için şuanki questler arasında değilll quest tamamlanamadı"),
	       *CurrentQuest.ToString());
	return false;
}

TArray<FTaskRequirement> UQuestComponent::GetCurrentTaskRequirements(FGameplayTag TaskID)
{
	if (!QuestDataTable.LoadSynchronous())
	{
		return {};
	}
	FString ContexString = CurrentQuest.ToString();


	FQuestInfo* Row = QuestDataTable.Get()->FindRow<FQuestInfo>(CurrentQuest.GetTagName(), ContexString);

	if (Row)
	{
		TArray<FTaskData>& Tasks = Row->RequiredTasks;
		for (const auto& Task : Tasks)
		{
			if (Task.TaskID == TaskID)
			{
				return Task.TaskRequirements;
			}
		}
	}
	return {};
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

bool UQuestComponent::AddCompletedActionTag(FGameplayTag CompletedActionTag)
{
	if (HasMatchingGameplayTag(CompletedActionTag))
	{
		return false;
	}
	
	for (auto& Requirement : CurrentQuestTasks[ActiveTaskIndex].TaskRequirements)
	{
		if (Requirement.bIsCompleted == false && Requirement.bIsCountRequirement == false && Requirement.RequirementTag
			== CompletedActionTag)
		{
			CompletedActions.AddTag(CompletedActionTag);
			Requirement.bIsCompleted = true;
			RefreshQuest();
			return true;
		}
	}

	return false;
}

void UQuestComponent::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = CompletedActions;
}

bool UQuestComponent::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return CompletedActions.HasTag(TagToCheck);
}

bool UQuestComponent::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return CompletedActions.HasAll(TagContainer);
}

bool UQuestComponent::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return CompletedActions.HasAny(TagContainer);
}
