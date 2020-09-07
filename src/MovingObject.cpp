#include "MovingObject.h"

MovingObject::MovingObject(olc::PixelGameEngine* pge)
	: Object(pge){}

void MovingObject::Restart()
{
	position = initPos;
	newPos = initPos;

	initSpeed = speed;

	direction = initDir;
	newDir = initDir;

	tileID = initTileID;

	CheckDirection();
}

void MovingObject::SetMovement(const olc::vf2d& position, float speed, const olc::vi2d& direction)
{
	SetPosition(position);
	this->speed = speed;	
	this->direction = direction;

	initSpeed = speed;

	initPos = position;
	newPos = position;
	initDir = direction;
	newDir = direction;
}

void MovingObject::SetTileID(const olc::vi2d& tileID)
{
	this->tileID = tileID;
	this->initTileID = tileID;
}

void MovingObject::CheckDirection()
{
	if (direction.x ==  0 && direction.y == -1) velocity = { 0.0f,	-speed };
	if (direction.x == -1 && direction.y ==  0) velocity = { -speed, 0.0f };
	if (direction.x ==  0 && direction.y ==  1) velocity = { 0.0f,	speed };
	if (direction.x ==  1 && direction.y ==  0) velocity = { speed,	 0.0f };
}

//void MovingObject::Move(Level* level)
//{
//}
