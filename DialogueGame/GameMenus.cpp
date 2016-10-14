#include <fstream>
#include <algorithm>
#include "GameMenus.h"
#include "EnemyGenerator.h"

std::string BuildHeroFileName(const std::string& heroName)
{
	return heroName + ".dat";
}

void PrintBasicSkill(BasicSkill skill)
{
	std::cout << skill + 1 << ". " << skill << std::endl;
}


void AwardBonusPointsToPlayer(Utils& utils, GameState& state)
{
	utils.ClearScreen();
	int awardedPoints = 5;
	std::cout << "You've been awarded " << awardedPoints << " skill points." << std::endl;
	utils.Pause();

	while (awardedPoints > 0)
	{
		utils.ClearScreen();
		std::cout << "Please spend some points(" << awardedPoints << ") on the skills in the below list." << std::endl;
		std::cout << "Choose a pair of skill/value Or press 0 to finish." << std::endl;
		utils.PrintEmptyLine();
		utils.PrintEmptyLine();

		std::for_each(SkillBegin, SkillEnd, &PrintBasicSkill);

		int wantedSkill, wantedValue;
		std::cin >> wantedSkill;

		if (wantedSkill == 0) break;
		if (wantedSkill < Accuracy + 1 || wantedSkill > Strength + 1)
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
		state.hero->AddBasicSkill(static_cast<BasicSkill>(wantedSkill - 1), wantedValue);
	}

	utils.ClearScreen();
	std::cout << "Player after initial upgrade." << std::endl;
	utils.PrintEmptyLine();
	utils.PrintEmptyLine();

	std::cout << state.hero->FullStat();
}

SHOULD_WAIT_FOR_USER AddPlayer(Utils& utils, GameState& state)
{
	utils.ClearInputBuffer();

	static SkillCreator skillCreator;

	std::cout << "Please enter your name ..." << std::endl;

	std::string playerName;
	std::getline(std::cin, playerName);

	Character* playerCharacter = new Character(playerName, 5);

	playerCharacter->AddSkill(skillCreator.CreateStamina());
	playerCharacter->AddSkill(skillCreator.CreateHealth());

	utils.ClearScreen();
	std::cout << "Congratulations! New player have been created." << std::endl;
	utils.PrintEmptyLine();
	utils.PrintEmptyLine();

	std::cout << playerCharacter->FullStat();

	state.hero = std::shared_ptr<Character>(playerCharacter);

	utils.Pause();
	AwardBonusPointsToPlayer(utils, state);

	return true;
}

bool LoadPlayer(GameState& state, const std::string& heroName)
{
	std::ifstream heroFile(BuildHeroFileName(heroName));

	Character* hero = nullptr;

	if (heroFile.is_open())
	{
		hero = new Character();
		heroFile >> *hero;
	}

	state.hero = std::shared_ptr<Character>(hero);

	return hero != nullptr;
}

SHOULD_WAIT_FOR_USER LoadPlayer(Utils& utils, GameState& state)
{
	utils.ClearInputBuffer();
	std::cout << "Please enter hero name to load ..." << std::endl;

	std::string heroFileName;
	std::getline(std::cin, heroFileName);

	if (LoadPlayer(state, heroFileName))
	{
		utils.ClearScreen();
		std::cout << "Player " << state.hero->Name() << " have been loaded." << std::endl;
		utils.PrintEmptyLine();
		utils.PrintEmptyLine();

		std::cout << state.hero->FullStat();
	}
	else
	{
		std::cerr << "Unable to locate save file for hero " << heroFileName << std::endl;
	}

	return true;
}

SHOULD_WAIT_FOR_USER SavePlayer(Utils& utils, GameState& state)
{
	if (nullptr == state.hero)
	{
		std::cerr << "Unable to save. No hero stored in memory." << std::endl;
		return true;
	}

	std::ofstream heroFile(BuildHeroFileName(state.hero->Name()));
	if (!heroFile.bad())
	{
		heroFile << *(state.hero);
		std::cout << "Successfully saved player " << state.hero->Name() << std::endl;
	}
	else
	{
		std::cerr << "Unable to create save file." << std::endl;
	}

	return true;
}

SHOULD_WAIT_FOR_USER ShowPlayer(Utils& utils, GameState& state)
{
	if (nullptr == state.hero)
	{
		std::cerr << "Unable to show stats. No hero stored in memory." << std::endl;
	}
	else 
	{
		std::cout << state.hero->FullStat();
	}

	return true;
}

SHOULD_WAIT_FOR_USER GenerateEnemy(Utils& utils, GameState& state)
{
	static EnemyGenerator eGenerator;

	std::cout << "Please provide enemy level to generate..." << std::endl;

	int enemyLevel;
	std::cin >> enemyLevel;

	Character enemy = eGenerator.Generate(enemyLevel, 5);

	utils.ClearScreen();
	std::cout << enemy.Name() << " generated." << std::endl;
	utils.PrintEmptyLine();
	utils.PrintEmptyLine();

	std::cout << enemy.FullStat();

	return true;
}
