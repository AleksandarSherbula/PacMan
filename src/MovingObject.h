#pragma once

#include "Level.h"

class MovingObject : public Object
{
public:
	float speed;
	
	olc::vi2d direction;
	olc::vi2d initTileID;
protected:
	float initSpeed;
	olc::vf2d initPos;
	olc::vf2d newPos;
	olc::vi2d initDir;
	olc::vi2d newDir;

	olc::vf2d velocity;	

	Level* level;
public:
	MovingObject(olc::PixelGameEngine* pge);

	void SetMovement(const olc::vf2d& position, float speed,
		const olc::vi2d& direction);

	void SetTileID(const olc::vi2d& tileID);
	
	virtual void Restart();
	virtual void CheckDirection();
	virtual void Move(Level* level) = 0;
};