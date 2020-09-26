#include "Ghost.h"
#include <time.h>

Ghost::Ghost(olc::PixelGameEngine* pge, GhostHouse gh, float noPelletTimer, int pelletCounter)
	: MovingObject(pge)
{
	srand(time(NULL));

	mode = GhostMode::SCATTER;
	ghostHouse = gh;
	initGhostHouse = gh;
	invertDir = false;
	this->noPelletTimer = noPelletTimer;
	this->pelletCounter = pelletCounter;
}

void Ghost::ChangeMode(float fElapsedTime, bool& isEnergized)
{
	if (mode != GhostMode::FRIGHTENED)
		lastmode = mode;

	if (mode != GhostMode::FRIGHTENED && ghostHouse == GhostHouse::OUT)
		timer += fElapsedTime;

	if (timer > 7.0f && mode == GhostMode::SCATTER)
	{
		mode = GhostMode::CHASE;
		timer = 0.0f;
	}

	if (timer > 20.0f && mode == GhostMode::CHASE)
	{
		mode = GhostMode::SCATTER;
		timer = 0.0f;
	}

	if (isEnergized)
	{		
		mode = GhostMode::FRIGHTENED;
	}
}

void Ghost::ModeBehaviour(Level* level, const olc::vi2d& scatterTile, const olc::vi2d& chaseTile)
{
	switch (mode)
	{
		case GhostMode::SCATTER:
		{
			targetTile = scatterTile;
			break;
		}
		case GhostMode::CHASE:
		{
			targetTile = chaseTile;
			break;
		}
		case GhostMode::FRIGHTENED:
		{
			FrightenedMode(level);
			break;
		}
	}
}

void Ghost::CheckMode()
{
	if (position == olc::vf2d(17.5f, 14.5f) || position == olc::vf2d(15.5f, 13.5f) ||
		position == olc::vf2d(13.5f, 14.5f))
		ghostHouse = GhostHouse::IN;
	else if (position == olc::vf2d(15.5f, 11.0f))
		ghostHouse = GhostHouse::OUT;
}

void Ghost::GhostHouseMoving()
{
	if (initPos == olc::vf2d(15.5f, 13.5f))
	{
		if (cmp(position, { initPos.x, initPos.y + 1.0f }, 0.2f))
		{
			newDir = { 0, -1 };
		}
		else if (cmp(position, initPos, 0.2f))
		{
			newDir = { 0,  1 };
		}
	}
	else
	{
		if (cmp(position, { initPos.x, initPos.y - 1.0f }, 0.2f))
		{
			newDir = { 0, 1 };
		}
		else if (cmp(position, initPos, 0.2f))
		{
			newDir = { 0, -1 };
		}
	}
}

void Ghost::Move(Level* level)
{
	for (olc::vi2d& crossPos : level->crossOverPos)
	{
		olc::vf2d targetFromLeft = targetTile - olc::vf2d(newPos.x - 1, position.y);
		olc::vf2d targetFromRight = targetTile - olc::vf2d(newPos.x + 1, position.y);
		olc::vf2d targetFromUp = targetTile - olc::vf2d(position.x, newPos.y - 1);
		olc::vf2d targetFromDown = targetTile - olc::vf2d(position.x, newPos.y + 1);

		float upDistance = targetFromUp.mag();
		float leftDistance = targetFromLeft.mag();
		float downDistance = targetFromDown.mag();
		float rightDistance = targetFromRight.mag();

		float shortestDistance = 0.0f;

		if (cmp(newPos, crossPos) && prevCrossPos != crossPos)
		{
			if (level->WallOnSide(olc::vi2d(crossPos.x, crossPos.y - 1))) upDistance = 999.0f;
			if (level->WallOnSide(olc::vi2d(crossPos.x - 1, crossPos.y))) leftDistance = 999.0f;
			if (level->WallOnSide(olc::vi2d(crossPos.x, crossPos.y + 1))) downDistance = 999.0f;
			if (level->WallOnSide(olc::vi2d(crossPos.x + 1, crossPos.y))) rightDistance = 999.0f;

			if (direction.y == -1)
				shortestDistance = std::min(fminf(leftDistance, rightDistance), upDistance);
			else if (direction.x == -1)
				shortestDistance = std::min(leftDistance, fminf(upDistance, downDistance));
			else if (direction.y == 1)
				shortestDistance = std::min(fminf(leftDistance, rightDistance), downDistance);
			else if (direction.x == 1)
				shortestDistance = std::min(rightDistance, fminf(upDistance, downDistance));

			if (upDistance == shortestDistance &&
				direction.y != 1)
			{
				newDir = { 0, -1 };
			}
			else if (leftDistance == shortestDistance &&
				direction.x != 1)
			{
				newDir = { -1, 0 };
			}
			else if (downDistance == shortestDistance &&
				direction.y != -1)
			{
				newDir = { 0, 1 };
			}
			else if (rightDistance == shortestDistance &&
				direction.x != -1)
			{
				newDir = { 1, 0 };
			}
			prevCrossPos = crossPos;
		}
	}
}

void Ghost::WallHit(Level* level)
{
	if (velocity.y < 0)
	{
		if (level->WallOnSide(olc::vi2d(position.x, newPos.y)))
		{
			newPos.y = (int)newPos.y + 1;
		}
	}
	else if (velocity.x < 0)
	{
		if (level->WallOnSide(olc::vi2d(newPos.x, position.y)))
		{
			newPos.x = (int)newPos.x + 1;
		}
		if (level->GetTile(olc::vi2d(newPos.x, position.y)) == '.')
		{
			newPos.x = 30.0f;
		}
	}
	else if (velocity.y > 0)
	{
		if (level->WallOnSide(olc::vi2d(position.x, newPos.y + 1)))
		{
			newPos.y = (int)newPos.y;
		}
	}
	else if (velocity.x > 0)
	{
		if (level->WallOnSide(olc::vi2d(newPos.x + 1, position.y)))
		{
			newPos.x = (int)newPos.x;
		}
		if (level->GetTile(olc::vi2d(newPos.x + 1, position.y)) == '.')
		{
			newPos.x = 1.0f;
		}
	}
}

void Ghost::CheckDirection()
{
	if (direction.x == -1)
	{
		tileID.x = 0;
		velocity = { -speed, 0.0f };
	}
	else if (direction.x == 1)
	{
		tileID.x = 2;
		velocity = { speed, 0.0f };
	}
	else if (direction.y == -1)
	{
		tileID.x = 4;
		velocity = { 0.0f, -speed };
	}
	else if (direction.y == 1)
	{
		tileID.x = 6;
		velocity = { 0.0f, speed };
	}
}

void Ghost::FrightenedMode(Level* level)
{
	if (invertDir)
	{
		newDir *= -1;
		invertDir = false;
	}	

	for (olc::vi2d& crossPos : level->crossOverPos)
	{
		//std::cout << newDir << std::endl;
		if (cmp(newPos, crossPos) && prevCrossPos != crossPos)
		{
			int dirID = rand() % 4;
			std::cout << newDir << std::endl;
			if (direction.y != 1 && dirID == 0)
			{
				if (level->WallOnSide(olc::vi2d(crossPos.x, crossPos.y - 1)))
					dirID = GetRandomExcluding(0);
				else
					newDir = { 0, -1 };
			}
			if (direction.x != 1 && dirID == 1)
			{
				if (level->WallOnSide(olc::vi2d(crossPos.x - 1, crossPos.y)))
					dirID = GetRandomExcluding(1);
				else
					newDir = { -1, 0 };
			}
			if (direction.y != -1 && dirID == 2)
			{
				if (level->WallOnSide(olc::vi2d(crossPos.x, crossPos.y + 1)))
					dirID = GetRandomExcluding(2);
				else
					newDir = { 0, 1 };
			}
			if (direction.x != -1 && dirID == 3)
			{
				if (level->WallOnSide(olc::vi2d(crossPos.x + 1, crossPos.y)))
					dirID = GetRandomExcluding(3);
				else
					newDir = { 1, 0 };
			}
			prevCrossPos = crossPos;
			
			//
			//if (newDir.y == -1 && level->WallOnSide(olc::vi2d(crossPos.x, crossPos.y - 1))) newDir.y = 0;
			//if (newDir.x == -1 && level->WallOnSide(olc::vi2d(crossPos.x - 1, crossPos.y))) newDir.x = 0;
			//if (newDir.y ==  1 && level->WallOnSide(olc::vi2d(crossPos.x, crossPos.y + 1))) newDir.y = 0;
			//if (newDir.x == 1 && level->WallOnSide(olc::vi2d(crossPos.x + 1, crossPos.y)))	newDir.x = 0;
			//
			//if (newDir.x != 0 && newDir.y != 0)
			//{
			//	if (newDir.y != -1)
			//		newDir = { 0, 1 };
			//	else
			//		newDir = { 1, 0 };
			//}		
		}
	}
}

void Ghost::Update(float fElapsedTime, Level* level, Player* player, const olc::vi2d& scatterTile, const olc::vi2d& chaseTile)
{
	newPos = position + velocity * fElapsedTime;

	ChangeMode(fElapsedTime, player->energized);	
	
	ModeBehaviour(level, scatterTile, chaseTile);

	//if (mode == GhostMode::SCATTER)			std::cout << 0 << std::endl;
	//if (mode == GhostMode::CHASE)				std::cout << 1 << std::endl;
	//if (mode == GhostMode::FRIGHTENED)		std::cout << 2 << std::endl;

	// Ghost's speed and Frightened sprite can be activated at any time
	if (mode == GhostMode::FRIGHTENED)
	{
		speed = initSpeed / 2;
		tileID.y = 4;		
	}
	else
	{
		if (ghostHouse == GhostHouse::GOING_SIDE || ghostHouse == GhostHouse::GOING_UP)
			speed = initSpeed / 2;
		else
			speed = initSpeed;
	}	

	if (ghostHouse == GhostHouse::IN)
	{		
		if (level->noPelletTimer < noPelletTimer && level->pelletCounter < pelletCounter)
		{
			GhostHouseMoving();
		}
		else
		{
			ghostHouse = GhostHouse::GOING_SIDE;
		}
	}
	else if (ghostHouse == GhostHouse::GOING_SIDE)
	{
		if (cmp(position, { 15.5f, 14.0f }))
		{
			ghostHouse = GhostHouse::GOING_UP;
		}
		else
		{
			// Ghost goes to center of the house
			position.y = 14.0f;
			newDir = initDir;
		}
	}
	else if (ghostHouse == GhostHouse::GOING_UP)
	{
		if (cmp(position, { 15.5f, 11.0f }))
		{
			//Ghost is out of house
			position = { 15.5f, 11.0f };
			newDir = { -1, 0 };
			ghostHouse = GhostHouse::OUT;
			speed = 5.0f;
		}
		else
		{
			// Ghost is heading out
			newDir = { 0, -1 };
		}
	}
	else if (ghostHouse == GhostHouse::OUT)
	{
		WallHit(level);
		
		if (mode != GhostMode::FRIGHTENED)
			Move(level);
	}
	
	direction = newDir;
	if (ghostHouse == GhostHouse::IN || ghostHouse == GhostHouse::GOING_UP)
		position.y = newPos.y;
	else if (ghostHouse == GhostHouse::GOING_SIDE)
		position.x = newPos.x;
	else
		position = newPos;	

	CheckDirection();
	Animate(tileID.x, tileID.x + 2, fElapsedTime, 0.09f);
}

void Ghost::Restart()
{
	tileID = initTileID;
	position = initPos;
	
	if (initGhostHouse == GhostHouse::OUT)
		direction = { -1, 0 };
	else
		direction = initDir;

	speed = initSpeed;
	newPos = initPos;
	newDir = direction;
	
	ghostHouse = initGhostHouse;	

	mode = GhostMode::SCATTER;

	timer = 0.0f;

	CheckMode();
	CheckDirection();
}

int GetRandomExcluding(int num)
{
	int res = rand() % 4;
	if (res == num)
		res = GetRandomExcluding(num);
	else
		return res;
}
