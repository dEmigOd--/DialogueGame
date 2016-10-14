#pragma once
#include <iostream>
#include <map>

class ReadWriteToFile
{
protected:
	static const int SAVE_FILE_VERSION = 3;

	void IgnoreRestOfLine(std::istream& is) const
	{
		is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	virtual std::ostream& WriteToStream(std::ostream& os) const = 0;
	virtual std::istream& ReadFromStream(std::istream& is) = 0;
};
