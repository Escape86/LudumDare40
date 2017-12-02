#pragma once

#include <vector>

#pragma region Forward Declarations
class Player;
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
};