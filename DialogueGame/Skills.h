#pragma once

#include <string>
#include <map>
#include "Common.h"
#include "ReadWriteToFile.h"
#include "VectorReaderWriter.h"
#include "BasicSkills.h"

class Skill : public NamedObject, public ReadWriteToFile
{
protected:
	int baseLevel;
	int addedLevel;
	CharacterProfile baseSkills;

	using BSVectorReaderWriter = VectorReaderWriter<CharacterProfile, BasicSkill, BasicSkill::SkillBegin, BasicSkill::SkillEnd>;

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
		{
			*this - (-value);
		}
		else 
		{
			addedLevel += value;
		}

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
		{
			*this + (-value);
		}
		else
		{
			addedLevel -= addedLevel <= value ? addedLevel : value;
		}

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

	virtual std::ostream& WriteToStream(std::ostream& os) const
	{
		// write obj to stream
		os << Name() << std::endl;
		os << baseLevel << " " << addedLevel << std::endl;
		os << BSVectorReaderWriter(baseSkills);

		return os;
	}

	virtual std::istream& ReadFromStream(std::istream& is)
	{
		// read obj from stream
		std::string name;
		std::getline(is, name);

		UpdateName(name);
		is >> baseLevel >> addedLevel;

		BSVectorReaderWriter rw;
		is >> rw;

		baseSkills.swap(rw.Skills());

		return is;
	}
};

class SkillCreator {
	Skill CreateBaseSkill(BasicSkill skill, int baseLevel, int multiplier)
	{
		CharacterProfile profile(EmptySkillsBag());
		profile[skill] = multiplier;

		return Skill(ToString(skill), baseLevel, profile);
	}
public:

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
};
