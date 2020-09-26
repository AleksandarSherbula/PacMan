#pragma once
#include "olcPixelGameEngine.h"
#include "Object.h"

#include <string>
#include <array>

class Level : public Object
{
private:
	std::string Map;
	int levelWidth;
	int levelHeight;
	std::unique_ptr<Object> blackRects;
	bool visiblePowerPellet;
	float powerPelletFlickTime;
public:
	int pelletCounter;
	float noPelletTimer;
	std::array<olc::vi2d, 64> crossOverPos;
public:
	Level(olc::PixelGameEngine* pge);
	~Level();

	void Update(float fElapsedTime);

	char GetTile(const olc::vi2d& id);
	void SetTile(const olc::vi2d& id, char c);

	bool WallOnSide(const olc::vi2d& id);

	void Draw();

	void Restart();

};