// DialogueGame.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <memory>
#include <fstream>
#include "Character.h"
#include "Skills.h"
// check out Windows specific
#include <stdlib.h>

void PrintBasicSkill(BasicSkill skill)
{
	std::cout << skill + 1 << ". " << skill << std::endl;
}

class Game
{
private:
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

	std::string BuildHeroFileName(const std::string& heroName) const
	{
		return heroName + ".dat";
	}

	void ClearInputBuffer() const
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

public:
	void PrintWelcomeMessage() const
	{
		std::cout << "Welcome to DialogueGame!" << std::endl;
		PrintEmptyLine();
		PrintEmptyLine();
		Pause();
	}

	int PrintMainMenu() const
	{
		ClearScreen();
		std::cout << "Main Menu" << std::endl;
		PrintEmptyLine();
		std::cout << "1. Create New Hero" << std::endl;
		std::cout << "2. Load Hero" << std::endl;
		std::cout << "3. Save Last Hero" << std::endl;
		std::cout << "4. Exit" << std::endl;
		PrintEmptyLine();

		int choice;
		std::cin >> choice;

		if (choice < 1 || choice > 4)
			return 4;

		return choice;
	}

	Character* AddPlayerOption() const
	{
		ClearInputBuffer();
		ClearScreen();
		std::cout << "Please enter your name ..." << std::endl;

		std::string playerName;
		std::getline(std::cin, playerName);

		Character* playerCharacter = new Character(playerName);

		playerCharacter->AddSkill(CreateStamina());
		playerCharacter->AddSkill(CreateHealth());

		ClearScreen();
		std::cout << "Congratulations! New player have been created." << std::endl;
		PrintEmptyLine();
		PrintEmptyLine();

		std::cout << playerCharacter->FullStat();

		Pause();
		return playerCharacter;
	}

	Character* LoadPlayer() const
	{
		ClearInputBuffer();
		ClearScreen();
		std::cout << "Please enter hero name to load ..." << std::endl;

		std::string heroFileName;
		std::getline(std::cin, heroFileName);

		std::ifstream heroFile(BuildHeroFileName(heroFileName));

		Character* hero = nullptr;

		if (!heroFile.bad())
		{
			hero = new Character();
			heroFile >> *hero;

			ClearScreen();
			std::cout << "Player " << hero->Name() << " have been loaded." << std::endl;
			PrintEmptyLine();
			PrintEmptyLine();

			std::cout << hero->FullStat();
		}
		else
		{
			std::cerr << "Unable to locate save file for hero " << heroFileName << std::endl;
		}

		Pause();
		return hero;
	}

	void SavePlayer(Character* hero) const
	{
		ClearScreen();

		if (nullptr == hero)
		{
			std::cerr << "Unable to save. No hero stored in memory." << std::endl;
			Pause();
			return;
		}

		std::ofstream heroFile(BuildHeroFileName(hero->Name()));
		if (!heroFile.bad())
		{
			heroFile << *hero;
			std::cout << "Successfully saved player " << hero->Name() << std::endl;
		}
		else
		{
			std::cerr << "Unable to create save file." << std::endl;
		}

		Pause();
	}

	void AwardBonusPointsToPlayer(Character& playerCharacter) const
	{
		ClearScreen();
		int awardedPoints = 5;
		std::cout << "You've been awarded " << awardedPoints << " skill points." << std::endl;
		PrintEmptyLine();

		while (awardedPoints > 0)
		{
			PrintEmptyLine();
			std::cout << "Please spend some points(" << awardedPoints << ") on the skills in the below list." << std::endl;
			std::cout << "Choose a pair of skill/value Or press 0 to finish." << std::endl;
			PrintEmptyLine();
			PrintEmptyLine();

			std::for_each(BasicSkill::SkillBegin, BasicSkill::SkillEnd, &PrintBasicSkill);

			int wantedSkill, wantedValue;
			std::cin >> wantedSkill;

			if (wantedSkill == 0) break;
			if (wantedSkill < BasicSkill::Accuracy + 1 || wantedSkill > BasicSkill::Strength + 1)
			{
				std::cerr << "Wrong skill picked." << std::endl;
				continue;
			}

			std::cin >> wantedValue;
			if (wantedValue <= 0 || wantedValue > awardedPoints)
			{
				std::cerr << "Wrong number of points tried to be spent." << std::endl;
				continue;
			}

			// ok everything is ok
			awardedPoints -= wantedValue;
			playerCharacter.AddBasicSkill(static_cast<BasicSkill>(wantedSkill - 1), wantedValue);
		}

		ClearScreen();
		std::cout << "Player after initial upgrade." << std::endl;
		PrintEmptyLine();
		PrintEmptyLine();

		std::cout << playerCharacter.FullStat();

		Pause();
	}
};

int main()
{
	Game game;

	game.PrintWelcomeMessage();

	std::shared_ptr<Character> player;

	while (true)
	{
		int mainMenuChoice = game.PrintMainMenu();
		if (mainMenuChoice == 4)
			break;

		switch (mainMenuChoice)
		{
		case 1:
			player = std::shared_ptr<Character>(game.AddPlayerOption());
			game.AwardBonusPointsToPlayer(*player);
			break;
		case 2:
			player = std::shared_ptr<Character>(game.LoadPlayer());
			break;
		case 3:
			game.SavePlayer(player.get());
			break;
		default:
			break;
		}
	}

	return 0;
}