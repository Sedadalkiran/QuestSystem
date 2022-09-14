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
	
	UPROPERTY()
	FGameplayTag CurrentQuestID;

	UPROPERTY()
	TMap<FGameplayTag,bool> Quests;

	UPROPERTY()
	TArray<FQuestRequiredMissions> Tasks;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	TSoftObjectPtr<UDataTable> QuestDataTable;

	UFUNCTION()
	void CompleteQuest();

	UFUNCTION()
	void TakeNewQuest();
	
	UFUNCTION()
	bool bIsCompletedTasks();

	UFUNCTION()
	TArray<FQuestRequiredMissions> GetRequiredMissions(FGameplayTag QuestID);
	
};
