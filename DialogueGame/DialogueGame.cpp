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
#include "ExperienceManager.h"
#include "Menu.h"
// check out Windows specific
#include <stdlib.h>

bool ExperienceManager::initialized;
int ExperienceManager::Levels[];

class Game : public ReadWriteToFile
{
private:
	Utils utils;
	GameState state;

	const char* AutosaveName = "Autosave.dat";

	Menu* CreateMainMenu() const
	{
		Menu* mainMenu = new ExitableMenu("Main Menu", Menu::OnCallCallback(Menu::DoNothing));

		Menu* createHero = new DeepestMenu("Create New Hero", Menu::OnCallCallback(AddPlayer));
		Menu* loadHero = new DeepestMenu("Load Hero", Menu::OnCallCallback(LoadPlayer));
		Menu* saveHero = new DeepestMenu("Save Hero", Menu::OnCallCallback(SavePlayer));
		Menu* showHero = new DeepestMenu("Show Hero", Menu::OnCallCallback(ShowPlayer));
		Menu* arena = new ExitableMenu("Enter Arena", Menu::OnCallCallback(EnterArena));

		mainMenu->AddSubMenu(createHero);
		mainMenu->AddSubMenu(loadHero);
		mainMenu->AddSubMenu(saveHero);
		mainMenu->AddSubMenu(showHero);
		mainMenu->AddSubMenu(arena);

		Menu* generateEnemy = new ExitableMenu("Generate Enemy", Menu::OnCallCallback(Menu::DoNothing));
		Menu* generateUnskilledEnemy = new DeepestMenu("Generate unskilled Enemy", Menu::OnCallCallback(GenerateDefaultEnemy));
		Menu* generateWarriorEnemy = new DeepestMenu("Generate Warrior", Menu::OnCallCallback(GenerateWarriorEnemy));
		Menu* generateRogueEnemy = new DeepestMenu("Generate Rogue", Menu::OnCallCallback(GenerateRogueEnemy));
		Menu* generateWizardEnemy = new DeepestMenu("Generate Wizard", Menu::OnCallCallback(GenerateWizardEnemy));

		generateEnemy->AddSubMenu(generateUnskilledEnemy);
		generateEnemy->AddSubMenu(generateWarriorEnemy);
		generateEnemy->AddSubMenu(generateRogueEnemy);
		generateEnemy->AddSubMenu(generateWizardEnemy);

		Menu* randomClash = new ExitableMenu("Random Clash", Menu::OnCallCallback(Menu::DoNothing));
		Menu* weakRival = new DeepestMenu("Weak rival", Menu::OnCallCallback(GenerateWeakFoe));
		Menu* equalRival = new DeepestMenu("Equal rival", Menu::OnCallCallback(GenerateEqualFoe));
		Menu* strongRival = new DeepestMenu("Strong rival", Menu::OnCallCallback(GenerateStrongFoe));

		randomClash->AddSubMenu(weakRival);
		randomClash->AddSubMenu(equalRival);
		randomClash->AddSubMenu(strongRival);

		arena->AddSubMenu(generateEnemy);
		arena->AddSubMenu(randomClash);

		return mainMenu;
	}

public:
	Game()
	{
		std::ifstream is(AutosaveName);
		if (is.is_open())
		{
			ReadFromStream(is);
		}
	}

	~Game()
	{
		SavePlayer(utils, state);
		WriteToStream(std::ofstream(AutosaveName));
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
		auto mainMenu = std::auto_ptr<Menu>(CreateMainMenu());

		while (mainMenu->OnCall(utils, state));
	}

	virtual std::ostream& WriteToStream(std::ostream& os) const
	{
		os << SAVE_FILE_VERSION << std::endl;
		// write current hero name
		os << (state.hero ? state.hero->Name() : "") << std::endl;

		return os;
	}

	virtual std::istream& ReadFromStream(std::istream& is)
	{
		int fileVersion;

		is >> fileVersion;
		_ASSERT_EXPR(fileVersion == SAVE_FILE_VERSION, "Unable to read save file of previous versions");

		IgnoreRestOfLine(is);

		// read obj from stream
		std::string name;
		std::getline(is, name);

		LoadPlayer(state, name);

		return is;
	}
};

inline std::ostream& operator<<(std::ostream& os, const Game& gm)
{
	return gm.WriteToStream(os);
}

inline std::istream& operator >> (std::istream& is, Game& gm)
{
	return gm.ReadFromStream(is);
}


int main()
{
	ExperienceManager::Initialize();

	Game game;

	game.PrintWelcomeMessage();
	game.DisplayMainMenu();

	return 0;
}