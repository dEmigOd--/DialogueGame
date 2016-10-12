#pragma once
#include <string>
#include <iostream>
#include <map>
#include "NamedObject.h"
#include "Skills.h"

class Character : public NamedObject, public ReadWriteToFile
{
private:
	static const int SAVE_FILE_VERSION = 3;

	std::map<std::string, Skill> skills;
	std::map<BasicSkill, int> basicSkills;
	int experience;
	int unassignedPoints;

public:
	Character()
	{
	}

	Character(const std::string& name, int unassignedPoints)
		: NamedObject(name), basicSkills(EmptySkillsBag()), experience(0), unassignedPoints(unassignedPoints)
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
		_ASSERT_EXPR(value <= unassignedPoints, "Not enough skill points to assign.");
		basicSkills.at(skill) += value;
		unassignedPoints -= value;
	}

	virtual void AddExperience(int addedExperience)
	{
		experience += addedExperience;
	}

	virtual int Experience() const
	{
		return experience;
	}

#define PRINT_BASE_SKILL(skill)  TAB() + ToString(skill) + "   " + TAB() + std::to_string(basicSkills.at(skill)) + ENDL()

	virtual std::string FullStat() const
	{
		std::string retValue = "Player Name: " + Name() + ENDL() +
			" Base skills: " + ENDL();
		
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
		os << SAVE_FILE_VERSION << std::endl;
		os << Name() << std::endl;

		for (auto const& skill : skills)
		{
			os << skill.first << std::endl;
			os << skill.second;
		}

		os << std::endl;
		os << BasicSkillVectorReaderWriter(basicSkills);
		os << Experience() << " " << unassignedPoints << std::endl;

		return os;
	}

	virtual std::istream& ReadFromStream(std::istream& is)
	{
		int fileVersion;

		is >> fileVersion;
		_ASSERT_EXPR(fileVersion == SAVE_FILE_VERSION, "Unable to read save file of previous versions");

		IgnoreRestOfLine(is);

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

			IgnoreRestOfLine(is);
			storedSkills.insert(std::make_pair(key, skill));
		}

		skills.swap(storedSkills);

		BasicSkillVectorReaderWriter rw;
		is >> rw;

		basicSkills.swap(rw.Skills());

		is >> experience >> unassignedPoints;

		return is;
	}
};

inline std::ostream& operator<<(std::ostream& os, const Character& ch)
{
	return ch.PrintToStream(os);
}

inline std::istream& operator>>(std::istream& is, Character& ch)
{
	return ch.ReadFromStream(is);
}
