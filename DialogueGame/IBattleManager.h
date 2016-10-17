#pragma once

#include "BattleResult.h"
#include "Character.h"

class IBattleManager
{
public:
	virtual BattleResult Clash(const Character& leftHero, const Character& rightHero) = 0;
};