#pragma once
#ifndef SKILLS_H
#define SKILLS_H

#include <string>
#include <map>
#include "Common.h"
#include "NamedObject.h"
#include "ReadWriteToFile.h"

enum BasicSkill : int
{
	Accuracy,
	SkillBegin = Accuracy,
	Dexterity,
	Speed,
	Stamina,
	Strength,
	SkillEnd,
};

std::string ToString(BasicSkill skill)
{
	switch (skill)
	{
		case Accuracy:
			return "Accuracy";
		case Dexterity:
			return "Dexterity";
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

std::ostream& operator<<(std::ostream& os,  BasicSkill skill)
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

BasicSkill &operator++(BasicSkill &c)
{
	c = static_cast<BasicSkill>(c + 1);
	return c;
}

BasicSkill operator++(BasicSkill &c, int)
{
	++c;
	return static_cast<BasicSkill>(c - 1);
}

BasicSkill &operator--(BasicSkill &c)
{
	return c = static_cast<BasicSkill>(c - 1);
}

BasicSkill operator--(BasicSkill &c, int)
{
	--c;
	return static_cast<BasicSkill>(c + 1);
}

BasicSkill operator*(BasicSkill c)
{
	return c;
}

std::map<BasicSkill, int> EmptySkillsBag()
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

	virtual std::ostream& PrintToStream(std::ostream& os) const
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

std::ostream& operator<<(std::ostream& os, const BasicSkillVectorReaderWriter& rw)
{
	return rw.PrintToStream(os);
}

std::istream& operator>>(std::istream& is, BasicSkillVectorReaderWriter& rw)
{
	return rw.ReadFromStream(is);
}


class Skill : public NamedObject, public ReadWriteToFile
{
protected:
	int baseLevel;
	int addedLevel;
	std::map<BasicSkill, int> baseSkills;

public:
	Skill()
	{ 
	}

	Skill(const std::string& name, int baseLevel, const std::map<BasicSkill, int>& baseCoefficients = std::map<BasicSkill, int>())
		: NamedObject(name), baseLevel(baseLevel), addedLevel(0), baseSkills(baseCoefficients)
	{ 
	}

	int SkillLevel(const std::map<BasicSkill, int>& characterSkills) const
	{
		int calculatedLevel = baseLevel;
		for (auto const& skill : baseSkills)
		{
			auto const ptrToCharacterSkill = characterSkills.find(skill.first);
			if (ptrToCharacterSkill != characterSkills.end())
			{
				calculatedLevel += skill.second * ptrToCharacterSkill->second;
			}
		}

		return calculatedLevel;
	}

	Skill& operator++(int)
	{
		Skill retValue(*this);
		++*this;
		return retValue;
	}

	Skill& operator++()
	{
		return *this + 1;
	}

	Skill& operator--(int)
	{
		Skill retValue(*this);
		--*this;
		return retValue;
	}

	Skill& operator--()
	{
		return *this - 1;
	}

	Skill& operator+=(int value)
	{
		if (value < 0)
			return *this - (-value);

		addedLevel += value;

		return *this;
	}

	Skill& operator+=(const Skill& other)
	{
		if(Name() != other.Name())
			throw std::logic_error("Unable to add different skills.");

		return *this += other.addedLevel;
	}

	Skill operator+(int value) const
	{
		Skill temp(*this);
		temp += value;
		return temp;
	}

	Skill& operator-=(int value)
	{
		if (value < 0)
			return *this + (-value);

		addedLevel -= addedLevel <= value ? addedLevel : value;

		return *this;
	}

	Skill operator-(int value) const
	{
		Skill temp(*this);
		temp -= value;
		return temp;
	}

	bool operator<(const Skill& other) const
	{
		return Name() < other.Name();
	}

	bool operator>(const Skill& other) const
	{
		return other < *this;
	}

	bool operator!=(const Skill& other) const
	{
		return (*this < other) || (*this > other);
	}

	bool operator==(const Skill& other) const
	{
		return !(*this != other);
	}

	virtual std::ostream& PrintToStream(std::ostream& os) const
	{
		// write obj to stream
		os << Name() << std::endl;
		os << baseLevel << " " << addedLevel << std::endl;
		os << BasicSkillVectorReaderWriter(baseSkills);

		return os;
	}

	virtual std::istream& ReadFromStream(std::istream& is)
	{
		// read obj from stream
		std::string name;
		std::getline(is, name);

		UpdateName(name);
		is >> baseLevel >> addedLevel;

		BasicSkillVectorReaderWriter rw;
		is >> rw;

		baseSkills.swap(rw.Skills());

		return is;
	}
};

std::ostream& operator<<(std::ostream& os, const Skill& skill)
{
	return skill.PrintToStream(os);
}

std::istream& operator>>(std::istream& is, Skill& skill)
{
	return skill.ReadFromStream(is);
}

Skill CreateBaseSkill(BasicSkill skill, int baseLevel, int multiplier)
{
	std::map<BasicSkill, int> underlying(EmptySkillsBag());
	underlying[skill] = multiplier;

	return Skill(ToString(skill), baseLevel, underlying);
}

Skill CreateAccuracy()
{
	throw NotImplementedException();
	//return CreateBaseSkill(BasicSkill::Accuracy, "Accuracy");
}

Skill CreateDexterity()
{
	throw NotImplementedException();
	//return CreateBaseSkill(BasicSkill::Dexterity, "Dexterity");
}

Skill CreateSpeed()
{
	throw NotImplementedException();
	//return CreateBaseSkill(BasicSkill::Speed, "Speed");
}

Skill CreateStamina()
{
	return CreateBaseSkill(BasicSkill::Stamina, 100, 10);
}

Skill CreateStrength()
{
	throw NotImplementedException();
	//return CreateBaseSkill(BasicSkill::Strength, "Strength");
}

Skill CreateHealth()
{
	return CreateBaseSkill(BasicSkill::Strength, 100, 10);
}

#endif
