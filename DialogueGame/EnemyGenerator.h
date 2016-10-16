#pragma once

#include "Character.h"

class EnemyGenerator
{
public:
	Character Generate(int level, int ptsPerLevel) const;
	Character Generate(int level, int ptsPerLevel, const CharacterProfile& specificProfile) const;
};