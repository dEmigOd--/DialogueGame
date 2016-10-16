// Tests.cpp : Defines the entry point for the console application.
//

#include "../DialogueGame/Experience.h"
#include "TestableCharacter.h"

bool ExperienceManager::initialized;
int ExperienceManager::Levels[];

void Initialize()
{
	ExperienceManager::Initialize();
}

void TestExperienceManager()
{
	ExperienceManager mgr;

	Character* ch = &TestableCharacter();

	ch->AddExperience(0);
	_ASSERT(mgr.GetLevel(*ch) == 0);

	ch->AddExperience(1000);
	_ASSERT(mgr.GetLevel(*ch) == 1);

	ch->AddExperience(2400);
	_ASSERT(mgr.GetLevel(*ch) == 2);

	ch->AddExperience(4800);
	_ASSERT(mgr.GetLevel(*ch) == 3);

	ch->AddExperience(100000);
	_ASSERT(mgr.GetLevel(*ch) == 10);

	ch->AddExperience(std::numeric_limits<int>::max());
	_ASSERT(mgr.GetLevel(*ch) == 100);

	ch->AddExperience(4799);
	_ASSERT(mgr.GetLevel(*ch) == 2);

	ch->AddExperience(4801);
	_ASSERT(mgr.GetLevel(*ch) == 3);

}

void TestRightLevelExperienceCalculated()
{
	ExperienceManager mgr;

	_ASSERT_EXPR(0 == mgr.GetExperienceForLevel(0), "Failed to calculate level 0 experience.");
	_ASSERT_EXPR(1000 == mgr.GetExperienceForLevel(1), "Failed to calculate level 1 experience.");
	_ASSERT_EXPR(100000 == mgr.GetExperienceForLevel(10), "Failed to calculate level 10 experience");
	_ASSERT_EXPR(15000 == mgr.GetExperienceForLevel(5), "Failed to calculate level 5 experience");
	_ASSERT_EXPR(99100000 == mgr.GetExperienceForLevel(100), "Failed to calculate level 100 experience");
}

int main()
{
	Initialize();

	TestExperienceManager();
	TestRightLevelExperienceCalculated();

	std::cout << "Great Success. All tests passed" << std::endl;

	return 0;
}

