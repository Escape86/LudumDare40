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
		case 2:
			return createLevel2();
		case 3:
			return createLevel3();

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

	for (std::vector<QueuedText>::iterator it = this->queuedText.begin(); it != this->queuedText.end();)
	{
		if (it->isBeingShown)
		{
			if (it->durationToShow > -1 && elapsedGameTimeInMilliseconds >= (it->whenToShow + it->durationToShow))
			{
				//text is expired, remove it
				Display::RemoveText(it->idFromCreation);
				it = this->queuedText.erase(it);
				continue;
			}
		}
		else
		{
			if (elapsedGameTimeInMilliseconds >= it->whenToShow)
			{
				//time to show the text
				it->idFromCreation = Display::CreateText(it->text, it->x, it->y, it->fontSize);
				it->isBeingShown = true;
			}
		}

		++it;
	}
}

void Level::InjectKeyPress()
{
	if (this->levelNumber == 1)
		shouldAdvanceLevel = true;
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

const int Level::GetLevelNumber()
{
	return this->levelNumber;
}

bool Level::ShouldAdvanceLevel()
{
	return this->shouldAdvanceLevel;
}

#pragma endregion

#pragma region Private Methods

#pragma region Constructor
Level::Level(int levelNumber, int orbCapcityForThisLevel) : levelNumber(levelNumber), orbCapacityForPlayer(orbCapcityForThisLevel)
{
	shouldAdvanceLevel = false;
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

	for (QueuedEnemy& qe : this->queuedEnemies)
	{
		delete qe.enemy;
	}
	this->queuedEnemies.clear();

	for (QueuedText& qt : this->queuedText)
	{
		if(qt.isBeingShown)
		{
			Display::RemoveText(qt.idFromCreation);
		}
	}
	this->queuedText.clear();
}

Level* Level::createLevel1()
{
	const int LEVEL_NUMBER = 1;

	const int whenToShowStory = 1750;

	Level* l = new Level(LEVEL_NUMBER, 5);
	l->queuedText.push_back({ 100, 100, "Welcome to Elemental Balance", 0, -1, Display::FontSize::THIRTYFOUR, false, -1});

	l->queuedText.push_back({ 50, 200, "For as long as time can tell, the world has been in peace and",		whenToShowStory,		-1, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 50, 225, "harmony. This is thanks to the balance of the three elements",		whenToShowStory + 250,	-1, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 50, 250, "of creation: Fire, Water, and Earth. However, something has",		whenToShowStory + 500,	-1, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 50, 275, "disturbed the eternal harmony of the world and the elements are",	whenToShowStory + 750,	-1, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 50, 300, "fighting amongst themselves. Please help restore balance to the",	whenToShowStory + 1000, -1, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 50, 325, "elements so that our world may continue to thrive in tranquility.", whenToShowStory + 1250, -1, Display::FontSize::TWENTY, false, -1 });

	l->queuedText.push_back({ 250, 450, "Press any key to continue...",										whenToShowStory + 3000, -1, Display::FontSize::TWENTY, false, -1 });
	return l;
}

Level* Level::createLevel2()
{
	const int LEVEL_NUMBER = 2;

	Level* l = new Level(LEVEL_NUMBER, 5);

	const int waterShrineX = SCREEN_WIDTH / 2;
	const int waterShrineY = SCREEN_HEIGHT - 40;

	l->areaTriggers.push_back(new AreaTrigger(waterShrineX, waterShrineY, ElementType::WATER));


	return l;
}

Level* Level::createLevel3()
{
	const int LEVEL_NUMBER = 3;

	Level* l = new Level(LEVEL_NUMBER, 5);

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