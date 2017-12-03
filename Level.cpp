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

void Level::InjectFrame(unsigned int elapsedGameTimeInMilliseconds)
{
	//spawn any enemies that are ready
	for (std::vector<QueuedEnemy>::iterator it = this->queuedEnemies.begin(); it != this->queuedEnemies.end();)
	{
		if (elapsedGameTimeInMilliseconds >= it->spawnTime)
		{
			this->enemies.push_back(it->enemy);
			it = this->queuedEnemies.erase(it);
			continue;
		}

		++it;
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

const int Level::GetOrbCapacityForThisLevel()
{
	return this->orbCapacityForPlayer;
}

#pragma endregion

#pragma region Private Methods

#pragma region Constructor
Level::Level(int orbCapcityForThisLevel) : orbCapacityForPlayer(orbCapcityForThisLevel)
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
	Level* l = new Level(5);

	return l;
}

Level* Level::createLevel2()
{
	Level* l = new Level(5);

	const int waterShrineX = SCREEN_WIDTH / 2;
	const int waterShrineY = SCREEN_HEIGHT - 40;

	l->areaTriggers.push_back(new AreaTrigger(waterShrineX, waterShrineY, ElementType::WATER));


	return l;
}

Level* Level::createLevel3()
{
	Level* l = new Level(5);

	const int waterShrineX = SCREEN_WIDTH / 2;
	const int waterShrineY = 40;
	const int fireShrineX = 40;
	const int fireShrineY = SCREEN_HEIGHT - 40;
	const int plantShrineX = SCREEN_WIDTH - 40;
	const int plantShrineY = SCREEN_HEIGHT - 40;

	l->areaTriggers.push_back(new AreaTrigger(waterShrineX, waterShrineY, ElementType::WATER));
	l->areaTriggers.push_back(new AreaTrigger(fireShrineX, fireShrineY, ElementType::FIRE));
	l->areaTriggers.push_back(new AreaTrigger(plantShrineX, plantShrineY, ElementType::PLANT));

	l->queuedEnemies.push_back({ 0, new Enemy(waterShrineX - 50, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });
	l->queuedEnemies.push_back({ 0, new Enemy(waterShrineX - 25, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });
	l->queuedEnemies.push_back({ 0, new Enemy(waterShrineX + 0,  waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });
	l->queuedEnemies.push_back({ 0, new Enemy(waterShrineX + 25, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });
	l->queuedEnemies.push_back({ 0, new Enemy(waterShrineX + 50, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });

	l->queuedEnemies.push_back({ 1000, new Enemy(waterShrineX - 50, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });
	l->queuedEnemies.push_back({ 1000, new Enemy(waterShrineX - 25, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });
	l->queuedEnemies.push_back({ 1000, new Enemy(waterShrineX + 0,  waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });
	l->queuedEnemies.push_back({ 1000, new Enemy(waterShrineX + 25, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });
	l->queuedEnemies.push_back({ 1000, new Enemy(waterShrineX + 50, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });

	return l;
}
#pragma endregion