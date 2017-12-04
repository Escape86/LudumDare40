#include "Level.h"
#include "AreaTrigger.h"
#include "Enemy.h"
#include "Texture.h"
#include "Constants.h"

#ifdef _DEBUG
	#include <assert.h>
#endif

#pragma region Public Methods
Level* Level::Load(int levelNumber, unsigned int loadTime)
{
	switch (levelNumber)
	{
		case 0:
			return createLevel0(loadTime);
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
		case 7:
			return createLevel7(loadTime);
		case 8:
			return createLevel8(loadTime);
		case 9:
			return createLevel9(loadTime);
		case 10:
			return createLevel10(loadTime);
		case 11:
			return createLevel11(loadTime);
		case 12:
			return createLevel12(loadTime);
		case 13:
			return createLevel13(loadTime);
		case 14:
			return createLevel14(loadTime);
		case 15:
			return createLevel15(loadTime);
		case 16:
			return createLevel16(loadTime);
		case 17:
			return createLevel17(loadTime);

		case GAMEOVER_LEVEL_ID:
			return createGameOverLevel(loadTime);

		default:
			return nullptr;
	}
}

void Level::InjectFrame(unsigned int elapsedLevelTimeInMilliseconds)
{
	//spawn any enemies that are ready
	for (std::vector<QueuedEnemy>::iterator it = this->queuedEnemies.begin(); it != this->queuedEnemies.end();)
	{
		if (elapsedLevelTimeInMilliseconds >= it->spawnTime)
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
		if (elapsedLevelTimeInMilliseconds >= it->spawnTime)
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
			if (it->durationToShow > -1 && elapsedLevelTimeInMilliseconds >= (it->whenToShow + it->durationToShow))
			{
				//text is expired, remove it
				Display::RemoveText(it->idFromCreation);
				it = this->queuedText.erase(it);
				continue;
			}
		}
		else
		{
			if (elapsedLevelTimeInMilliseconds >= it->whenToShow)
			{
				//time to show the text
				it->idFromCreation = Display::CreateText(it->text, it->x, it->y, it->fontSize, it->fontColor);
				it->isBeingShown = true;
			}
		}

		++it;
	}

	//YOLO draw queued textures defined by the level
	for (std::vector<QueuedTexture>::iterator it = this->queuedTextures.begin(); it != this->queuedTextures.end();)
	{
		if (it->isBeingShown)
		{
			if (it->durationToShow > -1 && elapsedLevelTimeInMilliseconds >= (it->whenToShow + it->durationToShow))
			{
				//texture is expired, remove it
				it = this->queuedTextures.erase(it);
				continue;
			}
			
			//draw it
			Display::QueueTextureForRendering(it->texture, it->x, it->y);
		}
		else
		{
			if (elapsedLevelTimeInMilliseconds >= it->whenToShow)
			{
				//time to show it
				it->isBeingShown = true;

				//draw it
				Display::QueueTextureForRendering(it->texture, it->x, it->y);
			}
		}

		++it;
	}

	if (this->levelNumber == 13 || this->levelNumber == 14 || this->levelNumber == 16)
	{
		if (this->queuedEnemies.size() == 0 && this->enemies.size() == 0)
		{
			this->shouldAdvanceLevel = true;
		}
	}
	else if (this->levelNumber == 8)
	{
		if (this->returnedOrbCount == 5)
		{
			this->shouldAdvanceLevel = true;
		}
	}
}

void Level::InjectKeyDown()
{
	if (!this->isKeyDownPrimed)
	{
		this->isKeyDownPrimed = true;
	}
}

void Level::InjectKeyUp()
{
	if (this->isKeyDownPrimed)
	{
		if (this->advancesOnKeyPress)
			this->shouldAdvanceLevel = true;
	}
}

void Level::InjectPlayerOrbCountChanged(int newOrbCount)
{
	//ghetto place to put level change logic... totally a hack :(
	if (this->levelNumber == 6)
	{
		if (newOrbCount == 1)
		{
			this->shouldAdvanceLevel = true;
		}
	}
	else if (this->levelNumber == 11)
	{
		if (newOrbCount == 0)
		{
			this->shouldAdvanceLevel = true;
		}
	}
}

void Level::InjectPlayerBroughtOrbToShrine(ElementType orbType, ElementType shrineType, int orbCount)
{
	if (orbType == shrineType)
		this->returnedOrbCount += orbCount;
}

void Level::InjectLevelEndTransitionBegin(unsigned int elapsedLevelTimeInMilliseconds)
{
	//remove any other text from the screen that the level may be showing
	for (QueuedText& qt : this->queuedText)
	{
		if (qt.isBeingShown)
		{
			Display::RemoveText(qt.idFromCreation);
		}
	}
	this->queuedText.clear();

	//pseudo random text (lols)
	this->queuedText.push_back({ 100, 250, elapsedLevelTimeInMilliseconds % 2 == 1 ? "Nice work!" : "Nicely done!", elapsedLevelTimeInMilliseconds + 1000, 2000, Display::FontSize::THIRTYFOUR, false, -1 });
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

const bool Level::GetUseTransition()
{
	return this->useTransition;
}

bool Level::ShouldAdvanceLevel()
{
	return this->shouldAdvanceLevel;
}

#pragma endregion

#pragma region Private Methods

#pragma region Constructor
Level::Level(int levelNumber, int startingNumberOfOrbs, int orbCapcityForThisLevel, ElementType startingElementType, unsigned int loadTime, bool advancesOnKeyPress, bool useTransition)
	: levelNumber(levelNumber), startingNumberOfOrbs(startingNumberOfOrbs), orbCapacityForPlayer(orbCapcityForThisLevel), startingElementType(startingElementType), loadTime(loadTime), advancesOnKeyPress(advancesOnKeyPress), useTransition(useTransition)
{
	this->shouldAdvanceLevel = false;
	this->isKeyDownPrimed = false;
	this->returnedOrbCount = 0;

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

	for (QueuedTexture& qt : this->queuedTextures)
	{
		if (qt.texture)
			delete qt.texture;
	}
	this->queuedTextures.clear();
}

Level* Level::createGameOverLevel(unsigned int loadTime)
{
	const int LEVEL_NUMBER = GAMEOVER_LEVEL_ID;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime, false, false);
	l->queuedText.push_back({ 100, 250, "Gameover... thanks for playing!", 0, -1, Display::FontSize::THIRTYFOUR, false, -1 });
	return l;
}

Level* Level::createLevel0(unsigned int loadTime)
{
	const int LEVEL_NUMBER = TITLE_LEVEL_ID;

	const int textDelay = 1000;

	Level* l = new Level(LEVEL_NUMBER, 0, 0, NONE, loadTime, true, false);

	Texture* t = new Texture("resources/Title.png");
	bool loaded = t->Load();
	assert(loaded);
	l->queuedTextures.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, -1, t, false });

	l->queuedText.push_back({ 250, SCREEN_HEIGHT - 40, "Press any key to continue...",				textDelay, -1, Display::FontSize::TWENTY, false, -1, WHITE });
	return l;
}

Level* Level::createLevel1(unsigned int loadTime)
{
	const int LEVEL_NUMBER = STORY_LEVEL_ID;

	const int textDelay = 1000;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime, true, false);
	l->queuedText.push_back({ 55, 225, "For as long as time can tell, the world has been in peace and",		textDelay,			-1, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 55, 250, "harmony. This is thanks to the balance of the three elements",		textDelay + 750,	-1, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 55, 275, "of creation: Fire, Water, and Earth. However, something has",		textDelay + 1500,	-1, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 55, 300, "disturbed the eternal harmony of the world and the elements are",	textDelay + 2250,	-1, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 55, 325, "fighting amongst themselves. Please help restore balance to the",	textDelay + 3000,	-1, Display::FontSize::TWENTY, false, -1 });
	l->queuedText.push_back({ 55, 350, "elements so that our world may continue to thrive in tranquility.", textDelay + 3750,	-1, Display::FontSize::TWENTY, false, -1 });

	l->queuedText.push_back({ 250, SCREEN_HEIGHT - 40, "Press any key to continue...",						textDelay, -1, Display::FontSize::TWENTY, false, -1, BLACK });
	return l;
}

Level* Level::createLevel2(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 2;

	const int textDelay = 1000;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime, true, false);

	Texture* t = new Texture("resources/Controls.png");
	bool loaded = t->Load();
	assert(loaded);
	l->queuedTextures.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, -1, t, false });

	l->queuedText.push_back({ 250, SCREEN_HEIGHT - 40, "Press any key to continue...",						textDelay, -1, Display::FontSize::TWENTY, false, -1, BLACK });

	return l;
}

Level* Level::createLevel3(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 3;

	const int textDelay = 1000;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime, true, false);

	Texture* t = new Texture("resources/Elements.png");
	bool loaded = t->Load();
	assert(loaded);
	l->queuedTextures.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, -1, t, false });

	l->queuedText.push_back({ 250, SCREEN_HEIGHT - 40, "Press any key to continue...",						textDelay, -1, Display::FontSize::TWENTY, false, -1, BLACK });

	return l;
}

Level* Level::createLevel4(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 4;

	const int textDelay = 1000;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime, true, false);

	Texture* t = new Texture("resources/Goal.png");
	bool loaded = t->Load();
	assert(loaded);
	l->queuedTextures.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, -1, t, false });

	l->queuedText.push_back({ 250, SCREEN_HEIGHT - 40, "Press any key to continue...",						textDelay, -1, Display::FontSize::TWENTY, false, -1, BLACK });

	return l;
	
}

Level* Level::createLevel5(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 5;

	const int textDelay = 1000;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime, true, false);

	Texture* t = new Texture("resources/OrbLimit1.png");
	bool loaded = t->Load();
	assert(loaded);
	l->queuedTextures.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, -1, t, false });

	l->queuedText.push_back({ 250, SCREEN_HEIGHT - 40, "Press any key to continue...",						textDelay, -1, Display::FontSize::TWENTY, false, -1, BLACK });

	return l;
}

Level* Level::createLevel6(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 6;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime, false, true);

	const int textDelay = 1000;

	const int waterOrbPositionX = 385;
	const int waterOrbPositionY = 233;
	l->queuedText.push_back({ 140, 225, "This is a water orb...",									textDelay,				6500, Display::FontSize::TWENTY, false, -1, BLACK });
	l->queuedEnemies.push_back({ textDelay + 1500, new Enemy(waterOrbPositionX, waterOrbPositionY, waterOrbPositionX, waterOrbPositionY, ElementType::WATER) });
	l->queuedText.push_back({ 140, 250, "Collecting it will atune you with water...",				textDelay + 3000,		5500, Display::FontSize::TWENTY, false, -1, BLACK });
	l->queuedText.push_back({ 140, 275, "See for yourself... collect the water orb...",				textDelay + 5500,		4500, Display::FontSize::TWENTY, false, -1, BLACK });

	return l;
}

Level* Level::createLevel7(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 7;

	const int textDelay = 1000;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime, true, false);

	Texture* t = new Texture("resources/OrbLimit2.png");
	bool loaded = t->Load();
	assert(loaded);
	l->queuedTextures.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, -1, t, false });

	l->queuedText.push_back({ 250, SCREEN_HEIGHT - 40, "Press any key to continue...",						textDelay, -1, Display::FontSize::TWENTY, false, -1, BLACK });

	return l;
}

Level* Level::createLevel8(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 8;

	const int textDelay = 1000;
	const int waterShrineX = SCREEN_WIDTH - 40;
	const int waterShrineY = SCREEN_HEIGHT - 40;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime, false, true);

	//section 1
	l->queuedText.push_back({ 100, 200, "Collect the orbs and return them to the shrine...",	textDelay,				-1, Display::FontSize::TWENTY, false, -1, BLACK });
	l->queuedAreaTriggers.push_back({ 0, new AreaTrigger(waterShrineX, waterShrineY, ElementType::WATER) });

	l->queuedEnemies.push_back({ 0, new Enemy(320, 400, 320, 400, ElementType::WATER) });
	l->queuedEnemies.push_back({ 0, new Enemy(360, 400, 360, 400, ElementType::WATER) });
	l->queuedEnemies.push_back({ 0, new Enemy(400, 400, 400, 400, ElementType::WATER) });
	l->queuedEnemies.push_back({ 0, new Enemy(440, 400, 440, 400, ElementType::WATER) });
	l->queuedEnemies.push_back({ 0, new Enemy(480, 400, 480, 400, ElementType::WATER) });

	return l;
}

Level* Level::createLevel9(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 9;

	const int textDelay = 1000;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime, true, false);

	Texture* t = new Texture("resources/Weakness.png");
	bool loaded = t->Load();
	assert(loaded);
	l->queuedTextures.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, -1, t, false });

	l->queuedText.push_back({ 250, SCREEN_HEIGHT - 40, "Press any key to continue...",						textDelay, -1, Display::FontSize::TWENTY, false, -1, BLACK });

	return l;
}

Level* Level::createLevel10(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 10;

	const int textDelay = 1000;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime, true, false);

	Texture* t = new Texture("resources/Weakness2.png");
	bool loaded = t->Load();
	assert(loaded);
	l->queuedTextures.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, -1, t, false });

	l->queuedText.push_back({ 250, SCREEN_HEIGHT - 40, "Press any key to continue...",						textDelay, -1, Display::FontSize::TWENTY, false, -1, BLACK });

	return l;
}

Level* Level::createLevel11(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 11;

	Level* l = new Level(LEVEL_NUMBER, 1, 5, WATER, loadTime, false, true);

	const int textDelay = 1000;

	const int fireOrbPositionX = 700;
	const int fireOrbPositionY = 332;

	l->queuedText.push_back({ 100, 325, "Attack the fire orb!",	textDelay,		4000, Display::FontSize::TWENTY, false, -1, BLACK });
	l->queuedEnemies.push_back({ textDelay + 1000, new Enemy(fireOrbPositionX, fireOrbPositionY, fireOrbPositionX, fireOrbPositionY, ElementType::FIRE) });

	return l;
}

Level* Level::createLevel12(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 12;

	const int textDelay = 1000;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime, true, false);

	Texture* t = new Texture("resources/GoalInfo.png");
	bool loaded = t->Load();
	assert(loaded);
	l->queuedTextures.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, -1, t, false });

	l->queuedText.push_back({ 250, SCREEN_HEIGHT - 40, "Press any key to continue...",						textDelay, -1, Display::FontSize::TWENTY, false, -1, BLACK });

	return l;
}

Level* Level::createLevel13(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 13;

	const int textDelay = 1000;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime, false, true);

	//section 1
	l->queuedText.push_back({ 240, 100, "Remove all orbs from the field...",	textDelay,				-1, Display::FontSize::TWENTY, false, -1, BLACK });

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

Level* Level::createLevel14(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 14;

	const int textDelay = 1000;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime, false, true);

	l->queuedText.push_back({ 190, 100, "Protect the Earth Shrine from Fire Orbs",	textDelay,				-1, Display::FontSize::TWENTY, false, -1, BLACK });

	const int fireShrineX = 40;
	const int fireShrineY = SCREEN_HEIGHT - 40;
	const int plantShrineX = SCREEN_WIDTH - 40;
	const int plantShrineY = SCREEN_HEIGHT - 40;

	l->queuedAreaTriggers.push_back({ 0, new AreaTrigger(fireShrineX, fireShrineY, ElementType::FIRE) });
	l->queuedAreaTriggers.push_back({ 0, new AreaTrigger(plantShrineX, plantShrineY, ElementType::PLANT) });

	//friendly orbs
	int x1 = 300; int y1 = 200; const int x1Offset = 50;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::WATER) }); x1 += x1Offset;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::WATER) }); x1 += x1Offset;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::WATER) }); x1 += x1Offset;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::WATER) }); x1 += x1Offset;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::WATER) });

	//attacking orbs
	for (unsigned int i = 1; i < 10; i++)
	{
		int x2 = 100; int y2 = 400; const int x2Offset = 50;
		l->queuedEnemies.push_back({ i * 2000, new Enemy(x2, y2, plantShrineX, plantShrineY, ElementType::FIRE) }); x2 += x2Offset;
		l->queuedEnemies.push_back({ i * 2000, new Enemy(x2, y2, plantShrineX, plantShrineY, ElementType::FIRE) }); x2 += x2Offset;
		l->queuedEnemies.push_back({ i * 2000, new Enemy(x2, y2, plantShrineX, plantShrineY, ElementType::FIRE) }); x2 += x2Offset;
		l->queuedEnemies.push_back({ i * 2000, new Enemy(x2, y2, plantShrineX, plantShrineY, ElementType::FIRE) }); x2 += x2Offset;
		l->queuedEnemies.push_back({ i * 2000, new Enemy(x2, y2, plantShrineX, plantShrineY, ElementType::FIRE) });
	}

	return l;
}

Level* Level::createLevel15(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 15;

	const int textDelay = 1000;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime, true, false);

	Texture* t = new Texture("resources/Remember.png");
	bool loaded = t->Load();
	assert(loaded);
	l->queuedTextures.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, -1, t, false });

	l->queuedText.push_back({ 250, SCREEN_HEIGHT - 40, "Press any key to continue...",						textDelay, -1, Display::FontSize::TWENTY, false, -1, BLACK });

	return l;
}

Level* Level::createLevel16(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 16;

	const int textDelay = 1000;

	Level* l = new Level(LEVEL_NUMBER, 0, 10, NONE, loadTime, false, true);

	//section 1
	l->queuedText.push_back({ 240, 35, "Remove all orbs from the field...",	textDelay,				-1, Display::FontSize::TWENTY, false, -1, BLACK });

	int x1 = 300; int y1 = 275; const int stepAmountX1 = 20; const int stepAmountY1 = 15;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::PLANT) });	x1 -= stepAmountX1; y1 -= stepAmountY1;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::PLANT) });	x1 -= stepAmountX1; y1 -= stepAmountY1;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::PLANT) });	x1 -= stepAmountX1; y1 -= stepAmountY1;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::PLANT) });	x1 -= stepAmountX1; y1 -= stepAmountY1;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::PLANT) });	x1 -= stepAmountX1; y1 -= stepAmountY1;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::PLANT) });	x1 -= stepAmountX1; y1 -= stepAmountY1;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::PLANT) });	x1 -= stepAmountX1; y1 -= stepAmountY1;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::PLANT) });	x1 -= stepAmountX1; y1 -= stepAmountY1;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::PLANT) });	x1 -= stepAmountX1; y1 -= stepAmountY1;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, x1, y1, ElementType::PLANT) });

	//section 2
	int x2 = 160; int y2 = 95;
	for (int i = 0; i < 20; i++)
	{
		l->queuedEnemies.push_back({ 0, new Enemy(x2, y2, x2, y2, ElementType::WATER) });
		x2 += 25;
	}

	//section 3
	int x3 = 500; int y3 = 275; const int stepAmountX3 = 20; const int stepAmountY3 = 15;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::FIRE) });	x3 += stepAmountX3; y3 -= stepAmountY3;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::FIRE) });	x3 += stepAmountX3; y3 -= stepAmountY3;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::FIRE) });	x3 += stepAmountX3; y3 -= stepAmountY3;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::FIRE) });	x3 += stepAmountX3; y3 -= stepAmountY3;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::FIRE) });	x3 += stepAmountX3; y3 -= stepAmountY3;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::FIRE) });	x3 += stepAmountX3; y3 -= stepAmountY3;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::FIRE) });	x3 += stepAmountX3; y3 -= stepAmountY3;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::FIRE) });	x3 += stepAmountX3; y3 -= stepAmountY3;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::FIRE) });	x3 += stepAmountX3; y3 -= stepAmountY3;
	l->queuedEnemies.push_back({ 0, new Enemy(x3, y3, x3, y3, ElementType::FIRE) });

	//section 4
	int x4 = 300; int y4 = 400; const int stepAmountX4 = 20; const int stepAmountY4 = 15;
	l->queuedEnemies.push_back({ 0, new Enemy(x4, y4, x4, y4, ElementType::WATER) });	x4 -= stepAmountX4; y4 += stepAmountY4;
	l->queuedEnemies.push_back({ 0, new Enemy(x4, y4, x4, y4, ElementType::WATER) });	x4 -= stepAmountX4; y4 += stepAmountY4;
	l->queuedEnemies.push_back({ 0, new Enemy(x4, y4, x4, y4, ElementType::FIRE) });	x4 -= stepAmountX4; y4 += stepAmountY4;
	l->queuedEnemies.push_back({ 0, new Enemy(x4, y4, x4, y4, ElementType::FIRE) });	x4 -= stepAmountX4; y4 += stepAmountY4;
	l->queuedEnemies.push_back({ 0, new Enemy(x4, y4, x4, y4, ElementType::PLANT) });	x4 -= stepAmountX4; y4 += stepAmountY4;
	l->queuedEnemies.push_back({ 0, new Enemy(x4, y4, x4, y4, ElementType::PLANT) });	x4 -= stepAmountX4; y4 += stepAmountY4;
	l->queuedEnemies.push_back({ 0, new Enemy(x4, y4, x4, y4, ElementType::WATER) });	x4 -= stepAmountX4; y4 += stepAmountY4;
	l->queuedEnemies.push_back({ 0, new Enemy(x4, y4, x4, y4, ElementType::WATER) });	x4 -= stepAmountX4; y4 += stepAmountY4;
	l->queuedEnemies.push_back({ 0, new Enemy(x4, y4, x4, y4, ElementType::FIRE) });	x4 -= stepAmountX4; y4 += stepAmountY4;
	l->queuedEnemies.push_back({ 0, new Enemy(x4, y4, x4, y4, ElementType::FIRE) });

	//section 5
	int x5 = 160; int y5 = 575;
	for (int i = 0; i < 20; i += 3)
	{
		l->queuedEnemies.push_back({ 0, new Enemy(x5, y5, x5, y5, ElementType::PLANT) });
		x5 += 25;
		l->queuedEnemies.push_back({ 0, new Enemy(x5, y5, x5, y5, ElementType::WATER) });
		x5 += 25;
		l->queuedEnemies.push_back({ 0, new Enemy(x5, y5, x5, y5, ElementType::FIRE) });
		x5 += 25;
	}

	//section 6
	int x6 = 500; int y6 = 400; const int stepAmountX6 = 20; const int stepAmountY6 = 15;
	l->queuedEnemies.push_back({ 0, new Enemy(x6, y6, x6, y6, ElementType::FIRE) });	x6 += stepAmountX6; y6 += stepAmountY6;
	l->queuedEnemies.push_back({ 0, new Enemy(x6, y6, x6, y6, ElementType::WATER) });	x6 += stepAmountX6; y6 += stepAmountY6;
	l->queuedEnemies.push_back({ 0, new Enemy(x6, y6, x6, y6, ElementType::WATER) });	x6 += stepAmountX6; y6 += stepAmountY6;
	l->queuedEnemies.push_back({ 0, new Enemy(x6, y6, x6, y6, ElementType::WATER) });	x6 += stepAmountX6; y6 += stepAmountY6;
	l->queuedEnemies.push_back({ 0, new Enemy(x6, y6, x6, y6, ElementType::WATER) });	x6 += stepAmountX6; y6 += stepAmountY6;
	l->queuedEnemies.push_back({ 0, new Enemy(x6, y6, x6, y6, ElementType::WATER) });	x6 += stepAmountX6; y6 += stepAmountY6;
	l->queuedEnemies.push_back({ 0, new Enemy(x6, y6, x6, y6, ElementType::WATER) });	x6 += stepAmountX6; y6 += stepAmountY6;
	l->queuedEnemies.push_back({ 0, new Enemy(x6, y6, x6, y6, ElementType::WATER) });	x6 += stepAmountX6; y6 += stepAmountY6;
	l->queuedEnemies.push_back({ 0, new Enemy(x6, y6, x6, y6, ElementType::WATER) });	x6 += stepAmountX6; y6 += stepAmountY6;
	l->queuedEnemies.push_back({ 0, new Enemy(x6, y6, x6, y6, ElementType::PLANT) });

	return l;
}

Level* Level::createLevel17(unsigned int loadTime)
{
	const int LEVEL_NUMBER = 17;

	const int textDelay = 1000;

	Level* l = new Level(LEVEL_NUMBER, 0, 5, NONE, loadTime, false, true);

	l->queuedText.push_back({ 190, 100, "Protect the Shrines...",	textDelay,				3000, Display::FontSize::TWENTY, false, -1, BLACK });

	const int fireShrineX = SCREEN_WIDTH - 40;
	const int fireShrineY = (SCREEN_HEIGHT / 2) - 20;
	const int plantShrineX = 40;
	const int plantShrineY = (SCREEN_HEIGHT / 2) - 20;
	const int waterShrineX = (SCREEN_WIDTH / 2);
	const int waterShrineY = SCREEN_HEIGHT - 40;

	l->queuedAreaTriggers.push_back({ 0, new AreaTrigger(fireShrineX, fireShrineY, ElementType::FIRE) });
	l->queuedAreaTriggers.push_back({ 0, new AreaTrigger(plantShrineX, plantShrineY, ElementType::PLANT) });
	l->queuedAreaTriggers.push_back({ 0, new AreaTrigger(waterShrineX, waterShrineY, ElementType::WATER) });

	//section 1
	int x1 = 300; int y1 = 500; const int stepAmountX1 = 20; const int stepAmountY1 = 15;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, fireShrineX, fireShrineY, ElementType::WATER) }); x1 += stepAmountX1; y1 += stepAmountY1;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, fireShrineX, fireShrineY, ElementType::WATER) }); x1 += stepAmountX1; y1 += stepAmountY1;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, fireShrineX, fireShrineY, ElementType::WATER) }); x1 += stepAmountX1; y1 += stepAmountY1;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, fireShrineX, fireShrineY, ElementType::WATER) }); x1 += stepAmountX1; y1 += stepAmountY1;
	l->queuedEnemies.push_back({ 0, new Enemy(x1, y1, fireShrineX, fireShrineY, ElementType::WATER) });



	////attacking orbs
	//for (unsigned int i = 1; i < 10; i++)
	//{
	//	int x2 = 100; int y2 = 400; const int x2Offset = 50;
	//	l->queuedEnemies.push_back({ i * 2000, new Enemy(x2, y2, plantShrineX, plantShrineY, ElementType::FIRE) }); x2 += x2Offset;
	//	l->queuedEnemies.push_back({ i * 2000, new Enemy(x2, y2, plantShrineX, plantShrineY, ElementType::FIRE) }); x2 += x2Offset;
	//	l->queuedEnemies.push_back({ i * 2000, new Enemy(x2, y2, plantShrineX, plantShrineY, ElementType::FIRE) }); x2 += x2Offset;
	//	l->queuedEnemies.push_back({ i * 2000, new Enemy(x2, y2, plantShrineX, plantShrineY, ElementType::FIRE) }); x2 += x2Offset;
	//	l->queuedEnemies.push_back({ i * 2000, new Enemy(x2, y2, plantShrineX, plantShrineY, ElementType::FIRE) });
	//}

	return l;
}
#pragma endregion