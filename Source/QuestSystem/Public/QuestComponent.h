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
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FGameplayTag CurrentQuestID;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int CurrentQuestNumber=0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FGameplayTag> Quests;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FQuestRequiredMissions> Tasks;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FGameplayTag ActiveQuest;
	
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	TSoftObjectPtr<UDataTable> QuestDataTable;

	UFUNCTION()
	bool CompleteQuest(FGameplayTag QuestID);

	UFUNCTION()
	void TakeNewQuest();
	
	UFUNCTION()
	bool bIsCompletedTasks();

	UFUNCTION()
	TArray<FQuestRequiredMissions> GetRequiredMissions(FGameplayTag QuestID);
	
};
