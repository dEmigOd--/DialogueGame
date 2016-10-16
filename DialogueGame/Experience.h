#pragma once

#include "Character.h"

class ExperienceManager
{
	static const int LEVELNUM = 101;
	static int Levels[LEVELNUM];

	static bool initialized;
public:
	static void Initialize()
	{
		if (!initialized)
		{
			// well the magic formula is
			int As[] = { 0, 1000, -100, 100 };

			for (int i = 0; i < LEVELNUM; ++i)
			{
				Levels[i] = 0;
				for (int indx = sizeof(As) / sizeof(As[0]) - 1; indx >= 0; --indx)
				{
					Levels[i] *= i;
					Levels[i] += As[indx];
				}
			}

			initialized = true;
		}
	}

	int GetLevel(const Character& hero) const
	{
		_ASSERT(initialized);

		int mid, low = 0, high = LEVELNUM - 1;

		int heroExperience = hero.Experience();

		while (low <= high)
		{
			mid = low + (high - low) / 2;

			if (Levels[mid] == heroExperience)
				return mid;

			if (Levels[mid] < heroExperience)
			{
				low = mid + 1;
			}
			else
			{
				high = mid - 1;
			}
		}

		return high;
	}

	int GetExperienceForLevel(int level) const
	{
		_ASSERT(initialized);

		return Levels[level];
	}
};