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

	std::unique_ptr<Player> player = nullptr;
	std::unique_ptr<Ghost> ghost = nullptr;

	std::unique_ptr<Level> level = nullptr;
	std::unique_ptr<Object> deadPacman = nullptr;

	float wait;
	float waitDead;

	bool isDead;

	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
};