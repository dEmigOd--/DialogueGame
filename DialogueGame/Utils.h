#pragma once

#include <iostream>

class Utils
{
public:
	void PrintEmptyLine() const
	{
		std::cout << std::endl;
	}

	void Pause() const
	{
		system("pause");
	}

	void ClearScreen() const
	{
		system("cls");
	}

	void ClearInputBuffer() const
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
};