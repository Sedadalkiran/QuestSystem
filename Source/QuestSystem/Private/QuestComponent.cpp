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


void UQuestComponent::TakeNewQuest(FGameplayTag QuestID)
{
	if (bHasActiveQuest)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s questi alınamadı aktif quest : %s"), *QuestID.ToString(),
		       *CurrentQuest.ToString());
		return;
	}

	if (FinishedQuests.Contains(QuestID))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s questi alınamadı, bitmiş questler içerisinde yer alıyor"),
		       *QuestID.ToString(), *CurrentQuest.ToString());
		return;
	}

	if (!QuestDataTable.LoadSynchronous())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s questi alınamadı data table yüklenemedi"), *QuestID.ToString());
		return;
	}
	FString ContexString = QuestID.ToString();

	FQuestInfo* Row = QuestDataTable.Get()->FindRow<FQuestInfo>(QuestID.GetTagName(), ContexString);

	if (Row)
	{
		if (IsRequiredQuestsCompletedForQuest(QuestID))
		{
			//FGameplayTag::RequestGameplayTag(QuestID.GetTagName());
			CurrentQuest = QuestID;

			CurrentQuestTasks=Row->RequiredTasks;
			
			bHasActiveQuest = true;
			FGameplayTag Tagim = StartNextTask();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s questi alınamadı gerekli questler tamamlanmadı"), *QuestID.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s questi alınamadı row bulunamadı"), *QuestID.ToString());
	}
}

FGameplayTag UQuestComponent::StartNextTask()
{
	if (!CurrentQuestTasks.IsEmpty() && bHasActiveQuest)
	{
		if (ActiveTaskIndex + 1 > 0)
		{
			for (int32 i = 0; i <= ActiveTaskIndex; i++)
			{
				if (CurrentQuestTasks.IsValidIndex(i))
				{
					UE_LOG(LogTemp, Warning, TEXT("%s taski %s"),
					       *CurrentQuestTasks[i].TaskID.ToString(),
					       CurrentQuestTasks[i].IsAllRequirementsCompleted()? TEXT("true") : TEXT("false"));
					if (CurrentQuestTasks[i].IsAllRequirementsCompleted() == false)
					{
						return FGameplayTag::EmptyTag;
					}
				}
				else
				{
					return FGameplayTag::EmptyTag;
				}
			}
			ActiveTaskIndex++;
		}

		if (ActiveTaskIndex + 1 == 0)
		{
			ActiveTaskIndex++;
		}

		return CurrentQuestTasks[ActiveTaskIndex].TaskID;
	}
	return FGameplayTag::EmptyTag;
}


void UQuestComponent::UpdateQuestList()
{
}

bool UQuestComponent::CompleteTaskForCurrentQuest(FGameplayTag TaskID)
{
	return SetTaskCompletedForQuest(CurrentQuest, TaskID, true);;
}

bool UQuestComponent::CompleteQuest(FGameplayTag QuestID)
{
	for (const auto& TaskState : CurrentQuestTasks)
	{
		if (!TaskState.IsAllRequirementsCompleted())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s questi için gereken %s taskı tamamlanmadı"), *QuestID.ToString(),
			       *TaskState.TaskID.ToString());
			return false;
		}
	}

	if (CurrentQuest.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s questi tamamlandı sebebiii bilinmiyor"), *QuestID.ToString());
		bHasActiveQuest = false;
		FinishedQuests.AddUnique(QuestID);
		ActiveTaskIndex = -1;
		CurrentQuest = FGameplayTag::EmptyTag;
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("%s questi için şuanki questler arasında değilll quest tamamlanamadı"),
	       *QuestID.ToString());
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


bool UQuestComponent::SetTaskCompletedForQuest(FGameplayTag QuestID, FGameplayTag TaskID, bool bCompleted)
{
	if (CurrentQuest != QuestID)
	{
		return false;
	}
	if(CurrentQuestTasks[ActiveTaskIndex].TaskID==TaskID)
	{
		for (FTaskData& TaskState : CurrentQuestTasks)
		{
			if (TaskState.TaskID == TaskID && TaskState.IsAllRequirementsCompleted() == false)
			{
				//TODO: Buraya sonra bak 
				for(auto& Requirement: TaskState.TaskRequirements)
				{
					Requirement.bIsCompleted = bCompleted;
				}
				
				if (ActiveTaskIndex < CurrentQuestTasks.Num() - 1)
				{
					StartNextTask();
				}

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

bool UQuestComponent::AddCompletedActionTag(FGameplayTag CompletedActionTag)
{
	if (HasMatchingGameplayTag(CompletedActionTag))
	{
		return false;
	}
	CompletedActions.AddTag(CompletedActionTag);
	for(auto& Requirement:CurrentQuestTasks[ActiveTaskIndex].TaskRequirements)
	{
		if(Requirement.bIsCompleted != false && Requirement.bIsCountRequirement == false && Requirement.RequirementTag == CompletedActionTag)
		{
			Requirement.bIsCompleted=true;
		}
	}
	
	return true;
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
