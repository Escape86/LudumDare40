#pragma once

#include <vector>

#pragma region Forward Declarations
class Player;
class Enemy;
#pragma endregion

class Game
{
public:
	Game();
	~Game();

	void InjectFrame();
	void InjectKeyDown(int key);
	void InjectKeyUp(int key);

private:
	Player* player;
	std::vector<Enemy*> enemies;
};