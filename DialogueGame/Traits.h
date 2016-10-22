#pragma once

#include <string>
#include <map>

enum CharacterTraits
{
	TraitHealth,
	TraitBegin = TraitHealth,
	TraitStamina,
	TraitEnd,
};

inline std::string ToString(CharacterTraits skill)
{
	switch (skill)
	{
	case CharacterTraits::TraitHealth:
		return "Health";
	case CharacterTraits::TraitStamina:
		return "Stamina";
	default:
		return "Unknown";
	}
}

using Exhaustables = std::map<CharacterTraits, int>;

namespace std
{
	template<>
	struct iterator_traits<CharacterTraits>
	{
		typedef CharacterTraits  value_type;
		typedef int    difference_type;
		typedef CharacterTraits *pointer;
		typedef CharacterTraits &reference;
		typedef std::bidirectional_iterator_tag
			iterator_category;
	};
}

inline CharacterTraits &operator++(CharacterTraits &c)
{
	c = static_cast<CharacterTraits>(c + 1);
	return c;
}

inline CharacterTraits operator++(CharacterTraits &c, int)
{
	++c;
	return static_cast<CharacterTraits>(c - 1);
}

inline CharacterTraits &operator--(CharacterTraits &c)
{
	return c = static_cast<CharacterTraits>(c - 1);
}

inline CharacterTraits operator--(CharacterTraits &c, int)
{
	--c;
	return static_cast<CharacterTraits>(c + 1);
}

inline CharacterTraits operator*(CharacterTraits c)
{
	return c;
}

