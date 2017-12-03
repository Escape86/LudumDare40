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

	std::vector<Enemy*>& GetEnemies();
	std::vector<AreaTrigger*>& GetShrines();

private:
	static Level* createLevel1();

	Level();
	~Level();

	std::vector<Enemy*> enemies;
	std::vector<AreaTrigger*> areaTriggers;
};