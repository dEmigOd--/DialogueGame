#pragma once
#include <iostream>
#include <map>

class ReadWriteToFile
{
protected:
	void IgnoreRestOfLine(std::istream& is) const
	{
		is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	virtual std::ostream& PrintToStream(std::ostream& os) const = 0;
	virtual std::istream& ReadFromStream(std::istream& is) = 0;
};
