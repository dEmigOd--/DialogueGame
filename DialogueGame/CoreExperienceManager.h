#pragma once
#include <crtdbg.h>
#include "Experience.h"

class CoreExperienceManager
{
	static const int LEVELNUM = 101;
	static Experience Levels[LEVELNUM];

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

	int GetLevel(const Experience& heroExperience) const
	{
		_ASSERT_EXPR(initialized, "Static inititalization called");

		int mid, low = 0, high = LEVELNUM - 1;

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

	Experience GetExperienceForLevel(int level) const
	{
		_ASSERT_EXPR(initialized, "Static inititalization called");

		return Levels[level];
	}
};