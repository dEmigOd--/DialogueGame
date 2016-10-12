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

	static const std::string ENDL;
	static const std::string TAB;

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