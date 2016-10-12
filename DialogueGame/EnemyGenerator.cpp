#include "EnemyGenerator.h"
#include "BasicSkills.h"

//from http://stackoverflow.com/questions/33420781/random-number-generators-from-c-random-library-in-realistic-programs?rq=1
/**
* (P)seudo (R)andom (N)umber (G)enerator
*/
template<typename Type = int>
class PRNG
{
	// easier to use param_type
	using param_type = typename std::uniform_int_distribution<Type>::param_type;

	// store an instance of the generator/distribution in the class object
	std::mt19937 gen;
	std::uniform_int_distribution<Type> dis;

public:
	// seed generator when creating
	PRNG() : gen(std::random_device()()) {}

	Type get(Type from, Type to)
	{
		// only need to create a light weigt param_type each time
		return dis(gen, param_type{ from, to });
	}
};

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
