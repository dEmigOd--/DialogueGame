#include "RandomNumberGenerator.h"
#include "EnemyGenerator.h"
#include "BasicSkills.h"

Character EnemyGenerator::Generate(int level, int ptsPerLevel) const
{
	static PRNG<int> rng;

	std::string name("Enemy" + std::to_string(rng.get(100000000, 999999999)));
	int unassignedSkillPoints = level * ptsPerLevel;
	Character enemy(name, unassignedSkillPoints);

	int from = static_cast<int>(SkillBegin), to = static_cast<int>(SkillEnd) - 1;

	for (; unassignedSkillPoints > 0; --unassignedSkillPoints)
	{
		enemy.AddBasicSkill(static_cast<BasicSkill>(rng.get(from, to)), 1);
	}

	return enemy;
}
