// Tests.cpp : Defines the entry point for the console application.
//

#include "../DialogueGame/Experience.h"
#include "TestableCharacter.h"

bool ExperienceManager::initialized;
int ExperienceManager::Levels[];

void TestExperienceManager()
{
	ExperienceManager::Initialize();

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

int main()
{
	TestExperienceManager();

	return 0;
}

