#pragma once
#include <iostream>
#include <map>

class ReadWriteToFile
{
protected:
	virtual std::ostream& PrintToStream(std::ostream& os) const = 0;
	virtual std::istream& ReadFromStream(std::istream& is) = 0;
};
