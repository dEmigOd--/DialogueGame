#pragma once

#include <iostream>
#include <memory>
#include "IBattleManager.h"


class Duel
{
	BattleResult duelOutcome;
	Character& leftCombatant;
	Character& rightCombatant;

	bool happened = false;
public:
	Duel(Character& leftCombatant, Character& rightCombatant)
		: leftCombatant(leftCombatant), rightCombatant(rightCombatant)
	{
	}

	void Fight(IBattleManager*  manager)
	{
		duelOutcome = manager->Clash(leftCombatant, rightCombatant);

		leftCombatant.AddExperience(duelOutcome.leftPlayerGain);
		rightCombatant.AddExperience(duelOutcome.rightPlayerGain);

		happened = true;
	}

	std::ostream& PrintOutcome(std::ostream& os)
	{
		_ASSERT_EXPR(happened, "Duel not yet happened");

		os << "Winner: " << (duelOutcome.winner == BattleResult::LEFT_PLAYER ? leftCombatant.Name() :
			(duelOutcome.winner == BattleResult::RIGHT_PLAYER ? rightCombatant.Name() : "None")) << std::endl;
		os << leftCombatant.Name() << "'s experience gain: " << duelOutcome.leftPlayerGain << std::endl;

		return os;
	}
};

inline std::ostream& operator << (std::ostream& os, Duel duel)
{
	return duel.PrintOutcome(os);
}
