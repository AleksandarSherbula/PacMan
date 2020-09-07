#pragma once

#include "Object.h"
#include "Level.h"
#include "Player.h"
#include "Ghost.h"
#include "olcPixelGameEngine.h"

class Game : public olc::PixelGameEngine
{
public:
	Game();
	~Game();

	//Player* player;
	std::unique_ptr<Player> player;
	std::unique_ptr<Ghost> ghost;
	//Level* level;
	std::unique_ptr<Level> level;
	std::unique_ptr<Object> deadPacman;

	float wait;
	float waitDead;

	bool isDead;
	bool roundTwo;

	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
};