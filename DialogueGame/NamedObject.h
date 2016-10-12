#pragma once
#include <string>

class NamedObject
{
private:
	std::string name;

protected:
	NamedObject()
		: NamedObject("")
	{
	}

	NamedObject(const std::string& name)
		: name(name)
	{
	}

	static std::string ENDL()
	{
		return "\n";
	}

	static std::string TAB()
	{
		return "\t";
	}

	void UpdateName(const std::string& newName)
	{
		name = newName;
	}

public:
	const std::string& Name() const
	{
		return name;
	}
};