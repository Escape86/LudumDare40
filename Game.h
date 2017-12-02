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

private:
	Player* player;
};