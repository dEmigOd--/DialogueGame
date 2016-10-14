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
#include "GameMenus.h"
#include "EnemyGenerator.h"
#include "Experience.h"
#include "Menu.h"
// check out Windows specific
#include <stdlib.h>

bool ExperienceManager::initialized;
int ExperienceManager::Levels[];

class Game
{
private:
	Utils utils;
	GameState state;

	Menu* CreateMainMenu() const
	{
		Menu* mainMenu = new ExitableMenu("Main Menu", Menu::OnCallCallback(Menu::DoNothing));

		Menu* createHero = new DeepestMenu("Create New Hero", Menu::OnCallCallback(AddPlayer));
		Menu* loadHero = new DeepestMenu("Load Hero", Menu::OnCallCallback(LoadPlayer));
		Menu* saveHero = new DeepestMenu("Save Hero", Menu::OnCallCallback(SavePlayer));
		Menu* generateEnemy = new DeepestMenu("Generate Random Enemy", Menu::OnCallCallback(GenerateEnemy));

		mainMenu->AddSubMenu(createHero);
		mainMenu->AddSubMenu(loadHero);
		mainMenu->AddSubMenu(saveHero);
		mainMenu->AddSubMenu(generateEnemy);

		return mainMenu;
	}

public:
	~Game()
	{
		SavePlayer(utils, state);
	}

	void PrintWelcomeMessage() const
	{
		std::cout << "Welcome to DialogueGame!" << std::endl;
		utils.PrintEmptyLine();
		utils.PrintEmptyLine();
		utils.Pause();
	}

	void DisplayMainMenu()
	{
		std::shared_ptr<Menu> mainMenu = std::shared_ptr<Menu>(CreateMainMenu());

		while (mainMenu->OnCall(utils, state));
	}
};

int main()
{
	Game game;

	game.PrintWelcomeMessage();
	game.DisplayMainMenu();

	return 0;
}