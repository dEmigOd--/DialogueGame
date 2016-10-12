#pragma once

#include <random>
#include "Character.h"

class EnemyGenerator
{
public:
	Character Generate(int level, int ptsPerLevel) const;
};