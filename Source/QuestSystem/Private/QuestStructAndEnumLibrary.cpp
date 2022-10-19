#include "QuestStructAndEnumLibrary.h"

bool FTaskData::IsAllRequirementsCompleted() const
{
	for(const auto& Requirement:TaskRequirements)
	{
		if(Requirement.bIsCompleted == false)
		{
			return false;
		}
	}
	return true;
}
