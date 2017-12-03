#include "Level.h"
#include "AreaTrigger.h"
#include "Enemy.h"
#include "Constants.h"

#pragma region Public Methods
Level* Level::Load(int levelNumber)
{
	switch (levelNumber)
	{
		case 1:
			return createLevel1();

		default:
			return nullptr;
	}
}

std::vector<Enemy*>& Level::GetEnemies()
{
	return this->enemies;
}

std::vector<AreaTrigger*>& Level::GetShrines()
{
	return this->areaTriggers;
}

#pragma endregion

#pragma region Private Methods

#pragma region Constructor
Level::Level()
{

}
#pragma endregion

Level::~Level()
{
	for (Enemy* e : this->enemies)
	{
		delete e;
	}
	this->enemies.clear();

	for (AreaTrigger* at : this->areaTriggers)
	{
		delete at;
	}
	this->areaTriggers.clear();
}

Level* Level::createLevel1()
{
	Level* l = new Level();

	const int waterShrineX = SCREEN_WIDTH / 2;
	const int waterShrineY = 40;
	const int fireShrineX = 40;
	const int fireShrineY = SCREEN_HEIGHT - 40;
	const int plantShrineX = SCREEN_WIDTH - 40;
	const int plantShrineY = SCREEN_HEIGHT - 40;

	l->areaTriggers.push_back(new AreaTrigger(waterShrineX, waterShrineY, ElementType::WATER));
	l->areaTriggers.push_back(new AreaTrigger(fireShrineX, fireShrineY, ElementType::FIRE));
	l->areaTriggers.push_back(new AreaTrigger(plantShrineX, plantShrineY, ElementType::PLANT));

	l->enemies.push_back(new Enemy(waterShrineX - 50, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER));
	l->enemies.push_back(new Enemy(waterShrineX - 25, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER));
	l->enemies.push_back(new Enemy(waterShrineX + 0, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER));
	l->enemies.push_back(new Enemy(waterShrineX + 25, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER));
	l->enemies.push_back(new Enemy(waterShrineX + 50, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER));
	
	return l;
}


#pragma endregion