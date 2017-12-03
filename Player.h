#pragma once

#include "Object.h"

class Player : public Object
{
public:
	Player();
	~Player();

	void Draw() override;

	void InjectFrame() override;
	void OnKeyDown(int key);
	void OnKeyUp(int key);

	void ResetHorizontalVelocity();
	void ResetVerticalVelocity();

	void HandleElementCollision(ElementType typeFromCollision);
	int GetElementStrength();
	void SetElementStrength(int value);

	int GetHp();
	void SetHp(int hp);

private:
	int horizontalVelocity;
	int verticalVelocity;

	int elementStrength;
	int hp;

	Texture* overlayTexture;
};