#pragma once
#include <string>
#include <iostream>
#include <map>
#include "NamedObject.h"
#include "Skills.h"
#include "Traits.h"
#include "Experience.h"
#include "CoreExperienceManager.h"
#include "VectorReaderWriter.h"

static CoreExperienceManager expManager;

class Character : public NamedObject, public ReadWriteToFile
{
private:
	static const int SkillPointsPerLevel = 5;

	std::map<std::string, Skill> skills;
	CharacterProfile basicSkills;
	Exhaustables traits;
	Experience experience;
	int unassignedPoints;

	using CVectorReaderWriter = VectorReaderWriter<CharacterProfile, BasicSkill, BasicSkill::SkillBegin, BasicSkill::SkillEnd>;

public:
	Character()
	{
	}

	Character(const std::string& name, int unassignedPoints = SkillPointsPerLevel)
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

	int UnassignedSkillPoints() const
	{
		return unassignedPoints;
	}

	int SkillLevel(const BasicSkill& skill) const
	{
		return basicSkills.at(skill);
	}

	virtual void AddExperience(Experience addedExperience)
	{
		int currLevel = expManager.GetLevel(experience);
		experience += addedExperience;
		int nextLevel = expManager.GetLevel(experience);

		unassignedPoints += SkillPointsPerLevel * (nextLevel - currLevel);
	}

	virtual Experience Experience() const
	{
		return experience;
	}

#define PRINT_BASE_SKILL(skill)  TAB() + ToString(skill) + "   " + TAB() + std::to_string(basicSkills.at(skill)) + ENDL()

	virtual std::string FullStat() const
	{
		std::string retValue = "Player Name: " + Name() + ENDL() +
			" Experience: " + std::to_string(Experience()) + ENDL() +
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

	virtual std::ostream& WriteToStream(std::ostream& os) const
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
		os << CVectorReaderWriter(basicSkills);
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

		CVectorReaderWriter rw;
		is >> rw;

		basicSkills.swap(rw.Skills());

		is >> experience >> unassignedPoints;

		return is;
	}
};
