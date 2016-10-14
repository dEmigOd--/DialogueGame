#pragma once

#include <memory>
#include "Character.h"

class GameState
{
public:
	std::shared_ptr<Character> hero;
};