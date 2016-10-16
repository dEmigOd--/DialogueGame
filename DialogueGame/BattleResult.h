#pragma once
#include "Experience.h"

class BattleResult
{
public:
	enum WHOWON
	{
		LEFT_PLAYER,
		RIGHT_PLAYER,
		DRAW,
	};

	WHOWON winner;
	Experience leftPlayerGain;
	Experience rightPlayerGain;
};