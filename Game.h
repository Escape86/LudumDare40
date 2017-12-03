#pragma once

#include <vector>

#pragma region Forward Declarations
class Player;
class Enemy;
class AreaTrigger;
#pragma endregion

class Game
{
public:
	Game();
	~Game();

	void InjectFrame();
	void InjectKeyDown(int key);
	void InjectKeyUp(int key);
	void InjectControllerStickMovement(uint8_t axis, int16_t value);

private:
	Player* player;
	std::vector<Enemy*> enemies;
	std::vector<AreaTrigger*> areaTriggers;

	int playerElementStrengthTextId;
	int playerHpTextId;
};