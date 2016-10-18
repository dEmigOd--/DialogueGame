#pragma once

#include <memory>
#include "IBattleManager.h"
#include "RandomNumberGenerator.h"

class RandomBattleManager : public IBattleManager
{
	std::auto_ptr<IRNG<int>> dice;
public:
	RandomBattleManager(IRNG<int>* dice = new PRNG<int>());
	virtual BattleResult Clash(const Character& leftHero, const Character& rightHero);
};
