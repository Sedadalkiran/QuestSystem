#pragma once
#include "GameplayTagContainer.h"
#include "QuestStructAndEnumLibrary.h"
#include "Engine/DataTable.h"
#include "FQuestInfo.generated.h"


USTRUCT(BlueprintType)
struct FQuestInfo:public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag QuestID;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FGameplayTag QuestName;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	EArea QuestArea;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	EQuestCategory QuestCategory;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FText Description;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FGameplayTag> RequiredQuests;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FTaskData> RequiredTasks;
	
	
};
