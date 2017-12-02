#pragma once

#pragma region Forward Declarations
class Texture;
#pragma endregion

class Player
{
public:
	Player();
	~Player();

	void InjectFrame();
	void Draw();

private:
	int x;
	int y;

	Texture* texture;
};