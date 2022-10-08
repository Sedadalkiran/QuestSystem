// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQuestInfo.h"
#include "Components/ActorComponent.h"
#include "QuestComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUESTSYSTEM_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 CurrentQuestIndex=0;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bHasActiveQuest=false;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 ActiveTaskIndex=-1;

	UFUNCTION(BlueprintPure)
	FGameplayTag GetCurrentQuestID() const;

	UFUNCTION(BlueprintPure)
	void GetCurrentQuestTasks(TArray<FTaskState>& OutTasks);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TArray<FGameplayTag> CurrentQuests;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TArray<FGameplayTag> FinishedQuests;

	//Key: QuestID Value:Task Array
	TMap<FGameplayTag,TArray<FTaskState>> CurrentQuestTasks;
	
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	TSoftObjectPtr<UDataTable> QuestDataTable;
	
	UFUNCTION(BlueprintCallable)
	void TakeNewQuest(FGameplayTag QuestID);

	UFUNCTION(BlueprintCallable)
	FGameplayTag StartNextTask();

	UFUNCTION (BlueprintCallable)
	void UpdateQuestList();
	
	UFUNCTION(BlueprintCallable)
	bool CompleteTaskForCurrentQuest(FGameplayTag TaskID);

	UFUNCTION(BlueprintCallable)
	bool CompleteQuest(FGameplayTag QuestID);

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentTaskRequirements(FGameplayTag TaskID);
	
	UFUNCTION(BlueprintCallable)
	TArray<FTaskState> GetRequiredTasksForQuest(FGameplayTag QuestID);

	UFUNCTION(BlueprintCallable)
	bool SetTaskCompletedForQuest(FGameplayTag QuestID, FGameplayTag TaskID, bool bCompleted);

	UFUNCTION(BlueprintCallable)
	bool IsRequiredQuestsCompletedForQuest(FGameplayTag QuestID);
};
