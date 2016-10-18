#pragma once

#include "Character.h"
#include "CoreExperienceManager.h"

static CoreExperienceManager manager;

class ExperienceManager
{
public:

	int GetLevel(const Character& hero) const
	{
		return manager.GetLevel(hero.Experience());
	}

	Experience GetExperienceForLevel(int level) const
	{
		return manager.GetExperienceForLevel(level);
	}
};