#include <numeric>
#include "RandomNumberGenerator.h"
#include "EnemyGenerator.h"
#include "BasicSkills.h"
#include "Profiles.h"

Character EnemyGenerator::Generate(int level, int ptsPerLevel) const
{
	return Generate(level, ptsPerLevel, ProfileCreator::DefaultProfile());
}

std::vector<BasicSkill> CreateSkillDistribution(const CharacterProfile& specificProfile)
{
	CharacterProfile profile(specificProfile);

	for (auto skill = SkillBegin; skill < SkillEnd; ++skill)
	{
		++profile[skill];
	}

	std::vector<BasicSkill> skillDistribution(std::accumulate(profile.begin(), profile.end(), 0, 
		[](const int& previous, const std::pair<BasicSkill, int>& curr) -> int {return previous + curr.second; }));

	auto skill = profile.begin();

	for (int i = 0, j = 0; i < skillDistribution.capacity(); ++i)
	{
		while (skill != profile.end() && !(skill->second))
		{
			++skill;
		}

		skillDistribution[i] = skill->first;
		--(skill->second);
	}

	return skillDistribution;
}

Character EnemyGenerator::Generate(int level, int ptsPerLevel, const CharacterProfile& specificProfile) const
{
	static PRNG<int> rng;
	
	std::string name("Enemy" + std::to_string(rng.get(100000000, 999999999)));

	std::vector<BasicSkill> skillDistribution(CreateSkillDistribution(specificProfile));

	int unassignedSkillPoints = level * ptsPerLevel;
	Character enemy(name, unassignedSkillPoints);

	int from = 0, to = static_cast<int>(skillDistribution.size()) - 1;

	for (; unassignedSkillPoints > 0; --unassignedSkillPoints)
	{
		enemy.AddBasicSkill(skillDistribution[rng.get(from, to)], 1);
	}

	return enemy;
}
