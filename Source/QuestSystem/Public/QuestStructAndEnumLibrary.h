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
	None=0 UMETA(DisplayName="None"),
	Oba=1 UMETA(DisplayName="Oba"),
	Antakya=2 UMETA(DisplayName="Antakya")
};

UENUM(BlueprintType)
enum class ETaskType:uint8
{
	MainTask=0 UMETA(DisplayName="MainTask"),
	SecondaryTask=1 UMETA(DisplayName="SecondaryTask")
};

UENUM(BlueprintType)
enum class ETaskState:uint8
{
	Active=0 UMETA(DisplayName="Active"),
	Deactive=1 UMETA(DisplayName="Deactive"),
	Ready=2 UMETA(DisplayName="Ready")

};

USTRUCT(BlueprintType)
struct FQuestRequiredMissions
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag TaskID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETaskType TaskType;

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

