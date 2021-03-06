#include <fstream>
#include <algorithm>
#include "GameMenus.h"
#include "EnemyGenerator.h"
#include "RandomBattleManager.h"
#include "ExperienceManager.h"
#include "Battle.h"
#include "Profiles.h"

#define WAIT_FOR_USER(funcCall) (funcCall); return true

ExperienceManager eManager;
EnemyGenerator eGenerator;
static std::auto_ptr<IRNG<int>> IntDice = std::auto_ptr<IRNG<int>>(new PRNG<int>());

std::string BuildHeroFileName(const std::string& heroName)
{
	return heroName + ".dat";
}

void PrintBasicSkill(BasicSkill skill)
{
	std::cout << skill + 1 << ". " << skill << std::endl;
}


SHOULD_WAIT_FOR_USER AssignUnassignedPoints(Utils& utils, GameState& state)
{	
	if (state.hero->UnassignedSkillPoints() <= 0)
	{
		return false;
	}

	std::cout << "You've " << state.hero->UnassignedSkillPoints() << " unassigned skill points." << std::endl;
	utils.Pause();

	utils.ClearInputBuffer();

	while (state.hero->UnassignedSkillPoints() > 0)
	{
		utils.ClearScreen();
		std::cout << "Please spend some points(" << state.hero->UnassignedSkillPoints() << ") on the skills in the below list." << std::endl;
		std::cout << "Choose a pair of skill/value [in separated list] Or press 0 to assign later." << std::endl;
		utils.PrintEmptyLine();
		utils.PrintEmptyLine();

		std::for_each(SkillBegin, SkillEnd, &PrintBasicSkill);

		int wantedSkill, wantedValue;
		std::cin >> wantedSkill;

		if (wantedSkill == 0) break;
		if (wantedSkill < SkillBegin + 1 || wantedSkill > SkillEnd)
		{
			std::cerr << "Wrong skill picked." << std::endl;
			continue;
		}

		std::cin >> wantedValue;
		if (wantedValue <= 0 || wantedValue > state.hero->UnassignedSkillPoints())
		{
			std::cerr << "Wrong number of points tried to be spent." << std::endl;
			continue;
		}

		// ok everything is ok
		state.hero->AddBasicSkill(static_cast<BasicSkill>(wantedSkill - 1), wantedValue);
	}

	return false;
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

SHOULD_WAIT_FOR_USER EnterArena(Utils& utils, GameState& state)
{
	utils.ClearScreen();
	if (state.hero == nullptr)
	{
		std::cout << "Please create/load hero before proceeding to Arena" << std::endl;
		return true;
	}

	return false;
}

void ClashWithFoe(Utils& utils, GameState& state, int levelAdvancement)
{
	static std::auto_ptr<IBattleManager> manager(new RandomBattleManager());

	int enemyLevel = std::max(0, eManager.GetLevel(*state.hero) + levelAdvancement);
	Character enemy = eGenerator.Generate(enemyLevel, 5);

	Duel duel(*state.hero, enemy);

	utils.ClearScreen();
	std::cout << "Fight between " << state.hero->Name() << " and " << enemy.Name() << std::endl;
	utils.PrintEmptyLine();

	duel.Fight(manager.get());

	std::cout << "\tResult:" << std::endl;
	std::cout << duel;
}

SHOULD_WAIT_FOR_USER GenerateWeakFoe(Utils& utils, GameState& state)
{
	WAIT_FOR_USER(ClashWithFoe(utils, state, -IntDice->get(0, 2)));
}

SHOULD_WAIT_FOR_USER GenerateEqualFoe(Utils& utils, GameState& state)
{
	WAIT_FOR_USER(ClashWithFoe(utils, state, 0));
}

SHOULD_WAIT_FOR_USER GenerateStrongFoe(Utils& utils, GameState& state)
{
	WAIT_FOR_USER(ClashWithFoe(utils, state, IntDice->get(0, 3)));
}

SHOULD_WAIT_FOR_USER GenerateEnemy(Utils& utils, GameState& state)
{
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

void GenerateGeneralEnemy(Utils& utils, GameState& state, const CharacterProfile& profile)
{
	std::cout << "Please provide enemy level to generate..." << std::endl;

	int enemyLevel;
	std::cin >> enemyLevel;

	Character enemy = eGenerator.Generate(enemyLevel, 5, profile);

	utils.ClearScreen();
	std::cout << enemy.Name() << " generated." << std::endl;
	utils.PrintEmptyLine();
	utils.PrintEmptyLine();

	std::cout << enemy.FullStat();
}

SHOULD_WAIT_FOR_USER GenerateDefaultEnemy(Utils& utils, GameState& state)
{
	WAIT_FOR_USER(GenerateGeneralEnemy(utils, state, ProfileCreator::DefaultProfile()));
}

SHOULD_WAIT_FOR_USER GenerateWarriorEnemy(Utils& utils, GameState& state)
{
	WAIT_FOR_USER(GenerateGeneralEnemy(utils, state, ProfileCreator::WarriorProfile()));
}

SHOULD_WAIT_FOR_USER GenerateRogueEnemy(Utils& utils, GameState& state)
{
	WAIT_FOR_USER(GenerateGeneralEnemy(utils, state, ProfileCreator::RogueProfile()));
}

SHOULD_WAIT_FOR_USER GenerateWizardEnemy(Utils& utils, GameState& state)
{
	WAIT_FOR_USER(GenerateGeneralEnemy(utils, state, ProfileCreator::WizardProfile()));
}

#undef WAIT_FOR_USER
