#pragma once

#include <string>
#include <map>
#include "ReadWriteToFile.h"

enum BasicSkill : int
{
	Accuracy,
	SkillBegin = Accuracy,
	Agility,
	Dexterity,
	Durability,
	Intelligence,
	Reflexes,
	Speed,
	Stamina,
	Strength,
	SkillEnd,
};

inline std::string ToString(BasicSkill skill)
{
	switch (skill)
	{
	case BasicSkill::Accuracy:
		return "Accuracy";
	case BasicSkill::Agility:
		return "Agility";
	case BasicSkill::Dexterity:
		return "Dexterity";
	case BasicSkill::Durability:
		return "Durability";
	case BasicSkill::Intelligence:
		return "Intelligence";
	case BasicSkill::Reflexes:
		return "Reflexes";
	case BasicSkill::Speed:
		return "Speed";
	case BasicSkill::Stamina:
		return "Stamina";
	case BasicSkill::Strength:
		return "Strength";
	default:
		return "Unknown";
	}
}

inline std::ostream& operator<<(std::ostream& os, BasicSkill skill)
{
	return os << ToString(skill);
}

using CharacterProfile = std::map<BasicSkill, int> ;

namespace std
{
	template<>
	struct iterator_traits<BasicSkill>
	{
		typedef BasicSkill  value_type;
		typedef int    difference_type;
		typedef BasicSkill *pointer;
		typedef BasicSkill &reference;
		typedef std::bidirectional_iterator_tag
			iterator_category;
	};
}

inline BasicSkill &operator++(BasicSkill &c)
{
	c = static_cast<BasicSkill>(c + 1);
	return c;
}

inline BasicSkill operator++(BasicSkill &c, int)
{
	++c;
	return static_cast<BasicSkill>(c - 1);
}

inline BasicSkill &operator--(BasicSkill &c)
{
	return c = static_cast<BasicSkill>(c - 1);
}

inline BasicSkill operator--(BasicSkill &c, int)
{
	--c;
	return static_cast<BasicSkill>(c + 1);
}

inline BasicSkill operator*(BasicSkill c)
{
	return c;
}

inline CharacterProfile EmptySkillsBag()
{
	CharacterProfile bag;

	for (BasicSkill skill = BasicSkill::SkillBegin; skill < BasicSkill::SkillEnd; ++skill)
	{
		bag[skill] = 0;
	}

	return bag;
}

