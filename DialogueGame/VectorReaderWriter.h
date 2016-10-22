#pragma once
#include <iostream>
#include <map>

template<class TDictionary, class TKey, TKey begin, TKey end>
class VectorReaderWriter : public ReadWriteToFile
{
//public:
//	using TDictionary = std::map<TKey, int>;

private:
	TDictionary skills;

public:

	VectorReaderWriter()
	{
	}

	VectorReaderWriter(const TDictionary& skills)
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
		TDictionary readSkills;

		int bskill, value;

		for (TKey skill = begin; skill < end; ++skill)
		{
			is >> bskill >> value;

			readSkills.insert(std::make_pair(static_cast<TKey>(skill), value));
		}

		skills.swap(readSkills);

		return is;
	}

	TDictionary Skills() const
	{
		return skills;
	}
};
