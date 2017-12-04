#include "Level.h"
#include "AreaTrigger.h"
#include "Enemy.h"
#include "Constants.h"

#ifdef _DEBUG
	#include <assert.h>
#endif

#pragma region Public Methods
Level* Level::Load(int levelNumber, unsigned int loadTime)
{
	switch (levelNumber)
	{
		case 1:
			return createLevel1(loadTime);
		case 2:
			return createLevel2(loadTime);
		case 3:
			return createLevel3(loadTime);
		case 4:
			return createLevel4(loadTime);
		case 5:
			return createLevel5(loadTime);
		case 6:
			return createLevel6(loadTime);

		case GAMEOVER_LEVEL_ID:
			return createGameOverLevel(loadTime);

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

	//spawn any shrines that are ready
	for (std::vector<QueuedAreaTrigger>::iterator it = this->queuedAreaTriggers.begin(); it != this->queuedAreaTriggers.end();)
	{
		if (elapsedGameTimeInMilliseconds >= it->spawnTime)
		{
			this->areaTriggers.push_back(it->areaTrigger);
			it = this->queuedAreaTriggers.erase(it);
			continue;
		}

		++it;
	}

	//show / hide any text that needs it
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

	if (this->levelNumber == 5)
	{
		if (this->queuedEnemies.size() == 0 && this->enemies.size() == 0)
		{
			this->shouldAdvanceLevel = true;
		}
	}
}

void Level::InjectKeyPress()
{
	if (this->levelNumber == 1)
		this->shouldAdvanceLevel = true;
}

void Level::InjectPlayerOrbCountChanged(int newOrbCount)
{
	//ghetto place to put level change logic... totally a hack :(
	if (this->levelNumber == 2)
	{
		if (newOrbCount == 1)
		{
			this->shouldAdvanceLevel = true;
		}
	}
	else if (this->levelNumber == 3)
	{
		if (newOrbCount == 0)
		{
			this->shouldAdvanceLevel = true;
		}
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

const ElementType Level::GetStartingElementTypeThisLevel()
{
	return this->startingElementType;
}

const int Level::GetNumberStartingOrbsForThisLevel()
{
	return this->startingNumberOfOrbs;
}

const int Level::GetOrbCapacityForThisLevel()
{
	return this->orbCapacityForPlayer;
}

const int Level::GetLevelNumber()
{
	return this->levelNumber;
}

const unsigned int Level::GetLevelLoadTime()
{
	return this->loadTime;
}

bool Level::ShouldAdvanceLevel()
{
	return this->shouldAdvanceLevel;
}

#pragma endregion

#pragma region Private Methods

#pragma region Constructor
Level::Level(int levelNumber, int startingNumberOfOrbs, int orbCapcityForThisLevel, ElementType startingElementType, unsigned int loadTime)
	: levelNumber(levelNumber), startingNumberOfOrbs(startingNumberOfOrbs), orbCapacityForPlayer(orbCapcityForThisLevel), startingElementType(startingElementType), loadTime(loadTime)
{
	shouldAdvanceLevel = false;

	//safety check
#ifdef _DEBUG
	assert((startingNumberOfOrbs == 0 && startingElementType == ElementType::NONE) || (startingNumberOfOrbs > 0 && startingElementType != ElementType::NONE));
#endif
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

Level* Level::createGameOverLevel(unsigned int loadTime)
{
	const int LEVEL_NUMBER = GAMEOVER_LEVEL_ID;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime);
	l->queuedText.push_back({ 100, 250, "Gameover... thanks for playing!", 0, -1, Display::FontSize::THIRTYFOUR, false, -1 });
	return l;
}

Level* Level::createLevel1(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 1;

	const int whenToShowStory = 2000;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime);
	l->queuedText.push_back({ 99, 100, "Welcome to Elemental Balance", 0, -1, Display::FontSize::THIRTYFOUR, false, -1});

	l->queuedText.push_back({ 55, 225, "For as long as time can tell, the world has been in peace and",		whenToShowStory,		-1, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 55, 250, "harmony. This is thanks to the balance of the three elements",		whenToShowStory + 750,	-1, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 55, 275, "of creation: Fire, Water, and Earth. However, something has",		whenToShowStory + 1500,	-1, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 55, 300, "disturbed the eternal harmony of the world and the elements are",	whenToShowStory + 2250,	-1, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 55, 325, "fighting amongst themselves. Please help restore balance to the",	whenToShowStory + 3000, -1, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 55, 350, "elements so that our world may continue to thrive in tranquility.", whenToShowStory + 3750, -1, Display::FontSize::TWENTY, false, -1 });

	l->queuedText.push_back({ 250, 450, "Press any key to continue...",										whenToShowStory + 4500, -1, Display::FontSize::TWENTY, false, -1 });
	return l;
}

Level* Level::createLevel2(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 2;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime);

	const int textDelay1 = 1000;
	const int textDelay2 = 10000;
	const int textDelay3 = 17500;
	const int textDelay4 = 23000;

	//section 1
	l->queuedText.push_back({ 341, 250, "This is you...",											textDelay1,				5000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 120, 338, "you can move around with W,A,S,D or the Arrow keys...",	textDelay1 + 2500,		4000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 255, 363, "(or controller D-pad / Stick)...",							textDelay1 + 4250,		3500, Display::FontSize::TWENTY, false, -1 });

	//section 2
	l->queuedText.push_back({ 140, 225, "You are currently in balance with all the elements...",	textDelay2,				4000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 140, 250, "Which is why you have a neutral color...",					textDelay2 + 2500,		4000, Display::FontSize::TWENTY, false, -1 });

	//section 3
	l->queuedText.push_back({ 277, 250, "But you can change that...",								textDelay3,				4000, Display::FontSize::TWENTY, false, -1 });
	
	//section 4
	const int waterOrbPositionX = 385;
	const int waterOrbPositionY = 233;
	l->queuedText.push_back({ 140, 225, "This is a water orb...",									textDelay4,				6500, Display::FontSize::TWENTY, false, -1 });
	l->queuedEnemies.push_back({ textDelay4 + 1500, new Enemy(waterOrbPositionX, waterOrbPositionY, waterOrbPositionX, waterOrbPositionY, ElementType::WATER) });
	l->queuedText.push_back({ 140, 250, "Collecting it will atune you with water...",				textDelay4 + 3000,		5500, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 140, 275, "See for yourself... collect the water orb...",				textDelay4 + 5500,		4500, Display::FontSize::TWENTY, false, -1 });

	return l;
}

Level* Level::createLevel3(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 3;

	Level* l = new Level(LEVEL_NUMBER, 1, 5, WATER, loadTime);

	const int textDelay1 = 1000;
	const int textDelay2 = 14000;
	const int textDelay3 = 23000;
	const int textDelay4 = 35000;
	const int textDelay5 = 51000;
	const int textDelay6 = 65000;
	const int textDelay7 = 81000;

	const int fireOrbPositionX = 700;
	const int fireOrbPositionY = 332;

	//section 1
	l->queuedText.push_back({ 330, 200, "Nicely done...",											textDelay1,				6000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 150, 225, "You can only carry so many orbs at once...",				textDelay1 + 2000,		6000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 150, 250, "Your current orb count and carrying capacity are",			textDelay1 + 4000,		6000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 150, 275, "shown in the upper right corner of the screen.",			textDelay1 + 5000,		6000, Display::FontSize::TWENTY, false, -1 });

	//section 2
	l->queuedText.push_back({ 100, 250, "Additionaly, your health is shown in the upper left...",	textDelay2,				6000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 100, 275, "If it reaches zero, it's lights out!",						textDelay2 + 2000,		6000, Display::FontSize::TWENTY, false, -1 });

	//section 3
	l->queuedText.push_back({ 100, 250, "If you are carrying more than your capacity, you will",	textDelay3,				6000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 100, 275, "become over-charged and take damage every second!",		textDelay3 + 2000,		6000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 100, 300, "The more over-charged you are, the more damage you take,",	textDelay3 + 4000,		6000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 100, 325, "so be careful.",											textDelay3 + 6000,		6000, Display::FontSize::TWENTY, false, -1 });

	//section 4
	l->queuedText.push_back({ 100, 250, "You can only be attuned to one element at a time...",		textDelay4,				6000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 100, 275, "When you are neutral, picking up any orb will attune you",	textDelay4 + 2000,		6000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 100, 300, "to the orb's element. If you are already attuned, then",	textDelay4 + 4000,		6000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 100, 325, "picking up a different type of orb will have a different",	textDelay4 + 6000,		6000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 100, 350, "effect depending on its type.",							textDelay4 + 8000,		6000, Display::FontSize::TWENTY, false, -1 });

	//section 5
	l->queuedText.push_back({ 100, 250, "Each element is weak against another element:",			textDelay5,				6000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 100, 275, "Fire is weak to Water",									textDelay5 + 2000,		6000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 100, 300, "Water is weak to Earth",									textDelay5 + 4000,		6000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 100, 325, "Earth is weak to Fire",									textDelay5 + 6000,		6000, Display::FontSize::TWENTY, false, -1 });

	//section 6
	l->queuedText.push_back({ 100, 250, "If the orb you touch is weak to an orb your're",			textDelay6,				6500, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 100, 275, "carrying, they will destroy each other. However if your",	textDelay6 + 2000,		6500, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 100, 300, " orb is weak to the type you touch, then they will",		textDelay6 + 4000,		6500, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 100, 325, " destroy each other and damage you in the process!",		textDelay6 + 6000,		6500, Display::FontSize::TWENTY, false, -1 });

	//section 7
	l->queuedText.push_back({ 100, 250, "This is a good way to eliminate orbs on the field while",	textDelay7,				6000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 100, 275, "also preventing yourself from becoming overcharged!",		textDelay7 + 2000,		6000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 100, 300, "Try it for yourself... you currently hold a water orb",	textDelay7 + 4000,		6000, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 100, 325, "which means fire is weak to you. Attack this fire orb!",	textDelay7 + 6000,		6000, Display::FontSize::TWENTY, false, -1 });
	l->queuedEnemies.push_back({ textDelay7 + 7500, new Enemy(fireOrbPositionX, fireOrbPositionY, fireOrbPositionX, fireOrbPositionY, ElementType::FIRE) });

	return l;
}

Level* Level::createLevel4(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 4;

	const int textDelay1 = 1000;
	const int waterShrineX = SCREEN_WIDTH - 40;
	const int waterShrineY = SCREEN_HEIGHT - 40;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime);

	//section 1
	l->queuedText.push_back({ 100, 200, "Collect the orbs and return them to the shrine...",	textDelay1,				-1, Display::FontSize::TWENTY, false, -1 });
	l->queuedAreaTriggers.push_back({ 0, new AreaTrigger(waterShrineX, waterShrineY, ElementType::WATER) });

	l->queuedEnemies.push_back({ 0, new Enemy(320, 400, 320, 400, ElementType::WATER) });
	l->queuedEnemies.push_back({ 0, new Enemy(360, 400, 360, 400, ElementType::WATER) });
	l->queuedEnemies.push_back({ 0, new Enemy(400, 400, 400, 400, ElementType::WATER) });
	l->queuedEnemies.push_back({ 0, new Enemy(440, 400, 440, 400, ElementType::WATER) });
	l->queuedEnemies.push_back({ 0, new Enemy(480, 400, 480, 400, ElementType::WATER) });

	return l;
}

Level* Level::createLevel5(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 5;

	const int textDelay1 = 1000;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime);

	//section 1
	l->queuedText.push_back({ 240, 100, "Remove all orbs from the field...",	textDelay1,				-1, Display::FontSize::TWENTY, false, -1 });

	int x1 = 300; int y1 = 250;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::WATER) }); x1 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::FIRE) });  x1 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::WATER) }); x1 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::FIRE) });  x1 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::WATER) }); x1 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::FIRE) });
	
	int x2 = 300; int y2 = 450;
	l->queuedEnemies.push_back({ 0, new Enemy(x2, y2, x2, y2, ElementType::WATER) });	x2 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x2, y2, x2, y2, ElementType::FIRE) });	x2 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x2, y2, x2, y2, ElementType::WATER) });	x2 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x2, y2, x2, y2, ElementType::FIRE) });	x2 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x2, y2, x2, y2, ElementType::WATER) });	x2 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x2, y2, x2, y2, ElementType::FIRE) });

	int x3 = 175; int y3 = 350;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::FIRE) }); x3 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::FIRE) }); x3 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::FIRE) }); x3 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::FIRE) }); x3 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::FIRE) }); x3 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::FIRE) }); x3 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::WATER) }); x3 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::WATER) }); x3 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::WATER) }); x3 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::WATER) }); x3 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::WATER) }); x3 += 40;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::WATER) });
	return l;
}

Level* Level::createLevel6(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 6;

	const int textDelay1 = 1000;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime);

	l->queuedText.push_back({ 240, 100, "Level 6",	textDelay1,				-1, Display::FontSize::TWENTY, false, -1 });

	//const int waterShrineX = SCREEN_WIDTH / 2;
	//const int waterShrineY = 40;
	//const int fireShrineX = 40;
	//const int fireShrineY = SCREEN_HEIGHT - 40;
	//const int plantShrineX = SCREEN_WIDTH - 40;
	//const int plantShrineY = SCREEN_HEIGHT - 40;

	//l->queuedAreaTriggers.push_back({ 0, new AreaTrigger(waterShrineX, waterShrineY, ElementType::WATER) });
	//l->queuedAreaTriggers.push_back({ 0, new AreaTrigger(fireShrineX, fireShrineY, ElementType::FIRE)    });
	//l->queuedAreaTriggers.push_back({ 0, new AreaTrigger(plantShrineX, plantShrineY, ElementType::PLANT) });

	//l->queuedEnemies.push_back({ 0, new Enemy(waterShrineX - 50, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });
	//l->queuedEnemies.push_back({ 0, new Enemy(waterShrineX - 25, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });
	//l->queuedEnemies.push_back({ 0, new Enemy(waterShrineX + 0,  waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });
	//l->queuedEnemies.push_back({ 0, new Enemy(waterShrineX + 25, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });
	//l->queuedEnemies.push_back({ 0, new Enemy(waterShrineX + 50, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });

	//l->queuedEnemies.push_back({ 1000, new Enemy(waterShrineX - 50, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });
	//l->queuedEnemies.push_back({ 1000, new Enemy(waterShrineX - 25, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });
	//l->queuedEnemies.push_back({ 1000, new Enemy(waterShrineX + 0,  waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });
	//l->queuedEnemies.push_back({ 1000, new Enemy(waterShrineX + 25, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });
	//l->queuedEnemies.push_back({ 1000, new Enemy(waterShrineX + 50, waterShrineY + 100, fireShrineX, fireShrineY, ElementType::WATER) });

	return l;
}

#pragma endregion