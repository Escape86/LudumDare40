#pragma once

#include <vector>
#include "Display.h"

#pragma region Forward Declarations
class Enemy;
class AreaTrigger;
enum ElementType;
#pragma endregion

class Level
{
public:
	static Level* Load(int levelNumber, unsigned int loadTime);
	
	~Level();

	void InjectFrame(unsigned int elapsedGameTimeInMilliseconds);
	void InjectKeyPress();
	void InjectPlayerOrbCountChanged(int newOrbCount);

	std::vector<Enemy*>& GetEnemies();
	std::vector<AreaTrigger*>& GetShrines();

	const ElementType GetStartingElementTypeThisLevel();
	const int GetNumberStartingOrbsForThisLevel();
	const int GetOrbCapacityForThisLevel();
	const int GetLevelNumber();
	const unsigned int GetLevelLoadTime();

	bool ShouldAdvanceLevel();

private:
	static Level* createGameOverLevel(unsigned int loadTime);

	static Level* createLevel1(unsigned int loadTime);
	static Level* createLevel2(unsigned int loadTime);
	static Level* createLevel3(unsigned int loadTime);
	static Level* createLevel4(unsigned int loadTime);
	static Level* createLevel5(unsigned int loadTime);
	static Level* createLevel6(unsigned int loadTime);

	Level(int levelNumber, int startingNumberOfOrbs, int orbCapcityForThisLevel, ElementType startingElementType, unsigned int loadTime);

	std::vector<Enemy*> enemies;
	std::vector<AreaTrigger*> areaTriggers;

	const int levelNumber;
	const unsigned int loadTime;
	const ElementType startingElementType;
	const int startingNumberOfOrbs;
	const int orbCapacityForPlayer;

	struct QueuedEnemy
	{
		unsigned int spawnTime;	//in ms
		Enemy* enemy;
	};
	std::vector<QueuedEnemy> queuedEnemies;

	struct QueuedAreaTrigger
	{
		unsigned int spawnTime;	//in ms
		AreaTrigger* areaTrigger;
	};
	std::vector<QueuedAreaTrigger> queuedAreaTriggers;

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