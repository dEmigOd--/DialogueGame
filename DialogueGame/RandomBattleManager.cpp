#include "RandomBattleManager.h"

const int MIN_SCORE = -100;
const int MAX_SCORE = 100;
const int BASE_PROPORTION = 5;

const int MIN_ABSOLUTE_GAIN = 50;
const int MAX_ABSOLUTE_GAIN = 250;

RandomBattleManager::RandomBattleManager(IRNG<int>* dice)
	: dice(dice)
{
}

BattleResult RandomBattleManager::Clash(const Character& leftHero, const Character& rightHero) 
{
	BattleResult result;

	int battleScoreForLeftPlayer = 0;

	for (auto skill = SkillBegin; skill < SkillEnd; ++skill)
	{
		int weight = dice->get(MIN_SCORE, MAX_SCORE);

		battleScoreForLeftPlayer += weight * (leftHero.SkillLevel(skill) - rightHero.SkillLevel(skill));
	}

	int gain = dice->get(0, MAX_SCORE);
	int absoluteGain = dice->get(MIN_ABSOLUTE_GAIN, MAX_ABSOLUTE_GAIN);
	int totalExperience = leftHero.Experience() + rightHero.Experience();
	int winnerProportion = dice->get(0, BASE_PROPORTION);

	int baseGain = absoluteGain + totalExperience * gain / MAX_SCORE;
	int winnerGain = baseGain + baseGain * winnerProportion / BASE_PROPORTION;
	int loserGain = 3 * baseGain - winnerGain;

	if (battleScoreForLeftPlayer > 0)
	{
		result.winner = BattleResult::LEFT_PLAYER;
		result.leftPlayerGain = winnerGain;
		result.rightPlayerGain = loserGain;
	}
	else if (battleScoreForLeftPlayer < 0)
	{
		result.winner = BattleResult::RIGHT_PLAYER;
		result.leftPlayerGain = loserGain;
		result.rightPlayerGain = winnerGain;
	}
	else
	{
		result.winner = BattleResult::DRAW;
		result.leftPlayerGain = baseGain;
		result.rightPlayerGain = baseGain;
	}

	return result;
}