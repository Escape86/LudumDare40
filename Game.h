#pragma once

#pragma region Forward Declarations
class Player;
class Level;
#pragma endregion

class Game
{
public:
	Game();
	~Game();

	void InjectFrame();
	void InjectKeyDown(int key);
	void InjectKeyUp(int key);
	void InjectControllerStickMovement(unsigned char axis, short value);

	void LoadLevel(int levelNumber);

private:
	Player* player;
	Level* currentLevel;

	int playerOrbCountTextId;
	int playerHpTextId;

	unsigned int previousFrameEndTime;

	bool isLevelEnding;
	int levelEndTimer;
};