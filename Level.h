#pragma once

#include <vector>
#include "Display.h"

#pragma region Forward Declarations
class Enemy;
class AreaTrigger;
class Texture;
enum ElementType;
#pragma endregion

class Level
{
public:
	static Level* Load(int levelNumber, unsigned int loadTime);
	
	~Level();

	void InjectFrame(unsigned int elapsedLevelTimeInMilliseconds);
	void InjectKeyDown();
	void InjectKeyUp();
	void InjectPlayerOrbCountChanged(int newOrbCount);
	void InjectPlayerBroughtOrbToShrine(ElementType orbType, ElementType shrineType, int orbCount);
	void InjectLevelEndTransitionBegin(unsigned int elapsedLevelTimeInMilliseconds);

	std::vector<Enemy*>& GetEnemies();
	std::vector<AreaTrigger*>& GetShrines();

	const ElementType GetStartingElementTypeThisLevel();
	const int GetNumberStartingOrbsForThisLevel();
	const int GetOrbCapacityForThisLevel();
	const int GetLevelNumber();
	const unsigned int GetLevelLoadTime();
	const bool GetUseTransition();

	bool ShouldAdvanceLevel();

private:
	static Level* createGameOverLevel(unsigned int loadTime);

	static Level* createLevel0(unsigned int loadTime);
	static Level* createLevel1(unsigned int loadTime);
	static Level* createLevel2(unsigned int loadTime);
	static Level* createLevel3(unsigned int loadTime);
	static Level* createLevel4(unsigned int loadTime);
	static Level* createLevel5(unsigned int loadTime);
	static Level* createLevel6(unsigned int loadTime);
	static Level* createLevel7(unsigned int loadTime);
	static Level* createLevel8(unsigned int loadTime);
	static Level* createLevel9(unsigned int loadTime);
	static Level* createLevel10(unsigned int loadTime);
	static Level* createLevel11(unsigned int loadTime);
	static Level* createLevel12(unsigned int loadTime);
	static Level* createLevel13(unsigned int loadTime);
	static Level* createLevel14(unsigned int loadTime);
	static Level* createLevel15(unsigned int loadTime);
	static Level* createLevel16(unsigned int loadTime);
	static Level* createLevel17(unsigned int loadTime);

	Level(int levelNumber, int startingNumberOfOrbs, int orbCapcityForThisLevel, ElementType startingElementType, unsigned int loadTime, bool advancesOnKeyPress, bool useTransition);

	std::vector<Enemy*> enemies;
	std::vector<AreaTrigger*> areaTriggers;

	const int levelNumber;
	const unsigned int loadTime;
	const ElementType startingElementType;
	const int startingNumberOfOrbs;
	const int orbCapacityForPlayer;
	const bool advancesOnKeyPress;
	bool isKeyDownPrimed;
	int returnedOrbCount;
	bool useTransition;

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
		SDL_Color fontColor;
	};
	std::vector<QueuedText> queuedText;

	struct QueuedTexture
	{
		int x;
		int y;
		unsigned int whenToShow;
		int durationToShow;
		Texture* texture;
		bool isBeingShown;
	};
	std::vector<QueuedTexture> queuedTextures;

	bool shouldAdvanceLevel;
};