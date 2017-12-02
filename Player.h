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
	void OnKeyDown(int key);
	void OnKeyUp(int key);

private:
	int x;
	int y;
	int horizontalVelocity;
	int verticalVelocity;
	int width;
	int height;

	Texture* texture;
};