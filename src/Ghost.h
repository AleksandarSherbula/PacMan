#pragma once

#include "MovingObject.h"
#include "Level.h"
#include "Player.h"

enum class GhostMode
{
	SCATTER,
	CHASE,
	FRIGHTENED
};

enum class GhostHouse
{
	IN,
	GOING_SIDE,
	GOING_UP,
	OUT
};

class Ghost : public MovingObject
{
private:	
	olc::vi2d targetTile;	
	GhostHouse ghostHouse;
	GhostHouse initGhostHouse;
	float timer;
	olc::vf2d prevCrossPos;
	float noPelletTimer;
	int pelletCounter;
public:
	bool invertDir;
	GhostMode mode;
	GhostMode lastmode;
public:
	Ghost(olc::PixelGameEngine* pge, GhostHouse gh, float noPelletTimer, int pelletCounter);	

	void ChangeMode(float fElapsedTime, bool& isEnergized);
	void ModeBehaviour(Level* level, const olc::vi2d& scatterTile, const olc::vi2d& chaseTile);
	void CheckMode();

	void GhostHouseMoving();
	void Move(Level* level) override;
	void WallHit(Level* level);
	void CheckDirection() override;
	void FrightenedMode(Level* level);

	void Update(float fElapsedTime, Level* level, Player* player, const olc::vi2d& scatterTile, const olc::vi2d& chaseTile);

	void Restart() override;
};

int GetRandomExcluding(int num);