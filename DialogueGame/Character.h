#pragma once
#include <string>
#include <iostream>
#include <map>
#include "NamedObject.h"
#include "Skills.h"

class Character : public NamedObject, public ReadWriteToFile
{
private:
	std::map<std::string, Skill> skills;
	std::map<BasicSkill, int> basicSkills;

public:
	Character()
	{
	}

	Character(const std::string& name)
		: NamedObject(name), basicSkills(EmptySkillsBag())
	{
	}

	void AddSkill(const Skill& newSkill)
	{
		if (skills.find(newSkill.Name()) == skills.end())
		{
			skills.insert(std::make_pair(newSkill.Name(), newSkill));
		}
		else
		{
			skills[newSkill.Name()] += newSkill;
		}
	}

	void AddBasicSkill(BasicSkill skill, int value)
	{
		basicSkills.at(skill) += value;
	}

#define PRINT_BASE_SKILL(skill)  TAB + ToString(skill) + "   " + TAB + std::to_string(basicSkills.at(skill)) + ENDL

	virtual std::string FullStat() const
	{
		std::string retValue = "Player Name: " + Name() + ENDL +
			" Base skills: " + ENDL;
		
		for (BasicSkill skill = BasicSkill::SkillBegin; skill < BasicSkill::SkillEnd; ++skill)
		{
			if (basicSkills.find(skill) != basicSkills.end())
			{
				retValue += PRINT_BASE_SKILL(skill);
			}
		}

		return retValue;
	}

	virtual std::ostream& PrintToStream(std::ostream& os) const
	{
		// write obj to stream
		os << Name() << std::endl;

		for (auto const& skill : skills)
		{
			os << skill.first << std::endl;
			os << skill.second;
		}

		os << std::endl;
		os << BasicSkillVectorReaderWriter(basicSkills);

		return os;
	}

	virtual std::istream& ReadFromStream(std::istream& is)
	{
		// read obj from stream
		std::string name;
		std::getline(is, name);

		UpdateName(name);

		std::map<std::string, Skill> storedSkills;

		while (true)
		{
			std::string key;
			std::getline(is, key);

			if (key.empty())
				break;

			Skill skill;
			is >> skill;

			is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			storedSkills.insert(std::make_pair(key, skill));
		}

		skills.swap(storedSkills);

		BasicSkillVectorReaderWriter rw;
		is >> rw;

		basicSkills.swap(rw.Skills());

		return is;
	}
};

std::ostream& operator<<(std::ostream& os, const Character& ch)
{
	return ch.PrintToStream(os);
}

std::istream& operator>>(std::istream& is, Character& ch)
{
	return ch.ReadFromStream(is);
}
