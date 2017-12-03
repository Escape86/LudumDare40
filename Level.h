#pragma once

#include <vector>
#include "Display.h"

#pragma region Forward Declarations
class Enemy;
class AreaTrigger;
#pragma endregion

class Level
{
public:
	static Level* Load(int levelNumber);
	
	~Level();

	void InjectFrame(unsigned int elapsedGameTimeInMilliseconds);
	void InjectKeyPress();

	std::vector<Enemy*>& GetEnemies();
	std::vector<AreaTrigger*>& GetShrines();

	const int GetOrbCapacityForThisLevel();
	const int GetLevelNumber();

	bool ShouldAdvanceLevel();

private:
	static Level* createLevel1();
	static Level* createLevel2();
	static Level* createLevel3();

	Level(int levelNumber, int orbCapcityForThisLevel);

	std::vector<Enemy*> enemies;
	std::vector<AreaTrigger*> areaTriggers;

	struct QueuedEnemy
	{
		unsigned int spawnTime;	//in ms
		Enemy* enemy;
	};
	std::vector<QueuedEnemy> queuedEnemies;

	const int levelNumber;
	const int orbCapacityForPlayer;

	struct QueuedText
	{
		int x;
		int y;
		std::string text;
		unsigned int whenToShow;
		int durationToShow;
		Display::FontSize fontSize;
		bool isBeingShown;
		int idFromCreation;
	};
	std::vector<QueuedText> queuedText;

	bool shouldAdvanceLevel;
};