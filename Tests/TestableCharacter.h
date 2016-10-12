#pragma once

#include "../DialogueGame/Character.h"

class TestableCharacter : public Character
{
	int experience;
public :
	void AddExperience(int addedExperience)
	{
		experience = addedExperience;
	}

	int Experience() const
	{
		return experience;
	}
};