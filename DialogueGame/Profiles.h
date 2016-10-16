#pragma once

#include "BasicSkills.h"

class ProfileCreator
{
public:
	static CharacterProfile DefaultProfile()
	{
		return CharacterProfile();
	}

	static CharacterProfile WarriorProfile()
	{
		CharacterProfile profile;

		profile[Durability]	+= 1;
		profile[Stamina]	+= 1;
		profile[Strength]	+= 2;
	}

	static CharacterProfile RogueProfile()
	{
		CharacterProfile profile;

		profile[Agility] += 1;
		profile[Dexterity] += 1;
		profile[Reflexes] += 1;
		profile[Speed] += 1;
	}

	static CharacterProfile WizardProfile()
	{
		CharacterProfile profile;

		profile[Intelligence] += 3;
		profile[Reflexes] += 1;
	}
};