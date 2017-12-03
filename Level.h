#pragma once

#include <vector>

#pragma region Forward Declarations
class Enemy;
class AreaTrigger;
#pragma endregion

class Level
{
public:
	static Level* Load(int levelNumber);

	void InjectFrame(unsigned int elapsedGameTimeInMilliseconds);

	std::vector<Enemy*>& GetEnemies();
	std::vector<AreaTrigger*>& GetShrines();

	const int GetOrbCapacityForThisLevel();

private:
	static Level* createLevel1();
	static Level* createLevel2();
	static Level* createLevel3();

	Level(int orbCapcityForThisLevel);
	~Level();

	std::vector<Enemy*> enemies;
	std::vector<AreaTrigger*> areaTriggers;

	struct QueuedEnemy
	{
		unsigned int spawnTime;	//in ms
		Enemy* enemy;
	};
	std::vector<QueuedEnemy> queuedEnemies;

	const int orbCapacityForPlayer;
};