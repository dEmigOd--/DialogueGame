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
	case Accuracy:
		return "Accuracy";
	case Agility:
		return "Agility";
	case Dexterity:
		return "Dexterity";
	case Durability:
		return "Durability";
	case Intelligence:
		return "Intelligence";
	case Reflexes:
		return "Reflexes";
	case Speed:
		return "Speed";
	case Stamina:
		return "Stamina";
	case Strength:
		return "Strength";
	default:
		return "Unknown";
	}
}

inline std::ostream& operator<<(std::ostream& os, BasicSkill skill)
{
	return os << ToString(skill);
}



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

inline std::map<BasicSkill, int> EmptySkillsBag()
{
	std::map<BasicSkill, int> bag;

	for (BasicSkill skill = BasicSkill::SkillBegin; skill < BasicSkill::SkillEnd; ++skill)
	{
		bag[skill] = 0;
	}

	return bag;
}

class BasicSkillVectorReaderWriter : ReadWriteToFile
{
	std::map<BasicSkill, int> skills;
public:

	BasicSkillVectorReaderWriter()
	{
	}

	BasicSkillVectorReaderWriter(const std::map<BasicSkill, int>& skills)
		: skills(skills)
	{
	}

	virtual std::ostream& WriteToStream(std::ostream& os) const
	{
		for (auto const& skill : skills)
		{
			os << static_cast<int>(skill.first) << " " << skill.second << std::endl;
		}
		return os;
	}

	virtual std::istream& ReadFromStream(std::istream& is)
	{
		std::map<BasicSkill, int> readSkills;

		int bskill, value;

		for (BasicSkill skill = BasicSkill::SkillBegin; skill < BasicSkill::SkillEnd; ++skill)
		{
			is >> bskill >> value;

			readSkills.insert(std::make_pair(static_cast<BasicSkill>(skill), value));
		}

		skills.swap(readSkills);

		return is;
	}

	std::map<BasicSkill, int> Skills() const
	{
		return skills;
	}
};

inline std::ostream& operator<<(std::ostream& os, const BasicSkillVectorReaderWriter& rw)
{
	return rw.WriteToStream(os);
}

inline std::istream& operator >> (std::istream& is, BasicSkillVectorReaderWriter& rw)
{
	return rw.ReadFromStream(is);
}

