#pragma once
#include <iostream>
#include <map>

class ReadWriteToFile
{
protected:
	static const int SAVE_FILE_VERSION = 4;

	void IgnoreRestOfLine(std::istream& is) const
	{
		is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

public:
	virtual std::ostream& WriteToStream(std::ostream& os) const = 0;
	virtual std::istream& ReadFromStream(std::istream& is) = 0;
};

inline std::ostream& operator<<(std::ostream& os, const ReadWriteToFile& writable)
{
	return writable.WriteToStream(os);
}

inline std::istream& operator >> (std::istream& is, ReadWriteToFile& readable)
{
	return readable.ReadFromStream(is);
}

