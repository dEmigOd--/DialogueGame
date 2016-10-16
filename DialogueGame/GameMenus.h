#pragma once
#include "GameState.h"
#include "Utils.h"

typedef bool SHOULD_WAIT_FOR_USER;

SHOULD_WAIT_FOR_USER AddPlayer(Utils& utils, GameState& state);

SHOULD_WAIT_FOR_USER LoadPlayer(Utils& utils, GameState& state);

bool LoadPlayer(GameState& state, const std::string& heroName);

SHOULD_WAIT_FOR_USER SavePlayer(Utils& utils, GameState& state);

SHOULD_WAIT_FOR_USER ShowPlayer(Utils& utils, GameState& state);

SHOULD_WAIT_FOR_USER GenerateEnemy(Utils& utils, GameState& state);

SHOULD_WAIT_FOR_USER GenerateDefaultEnemy(Utils& utils, GameState& state);
SHOULD_WAIT_FOR_USER GenerateWarriorEnemy(Utils& utils, GameState& state);
SHOULD_WAIT_FOR_USER GenerateRogueEnemy(Utils& utils, GameState& state);
SHOULD_WAIT_FOR_USER GenerateWizardEnemy(Utils& utils, GameState& state);
