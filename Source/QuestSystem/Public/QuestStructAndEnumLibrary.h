#pragma once
#include "GameplayTagContainer.h"
#include "QuestStructAndEnumLibrary.generated.h"

UENUM(BlueprintType)
enum class EQuestCategory:uint8
{
	MainQuest=0 UMETA(DisplayName="MainQuest"),
	SideQuest=1 UMETA(DisplayName="SideQuest")
};

UENUM(BlueprintType)
enum class EArea:uint8
{
	Oba=0 UMETA(DisplayName="Oba"),
	Antakya=1 UMETA(DisplayName="Antakya")
};

USTRUCT(BlueprintType)
struct FQuestRequiredMissions
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag TaskID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag TaskType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag QuestID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsCompleted;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CurrentAmount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int RequiredAmount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText MissionDescription;
	
};

