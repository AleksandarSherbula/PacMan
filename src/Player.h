#pragma once

#include "MovingObject.h"

class Player : public MovingObject
{
public:
	bool energized = false;
	float energizedTimer;
public:
	Player(olc::PixelGameEngine* pge);

	void Update(float fElapsedTime, Level* level);
	
	void WallHit(Level* level);
	void Move(Level* level) override;
	void CheckDirection() override;

	void Restart() override;
};

bool cmp(olc::vf2d a, olc::vf2d b, float epsilon = 0.1f);