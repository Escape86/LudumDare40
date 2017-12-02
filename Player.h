#pragma once

#include "Object.h"

class Player : public Object
{
public:
	Player();
	~Player();

	void InjectFrame() override;
	void OnKeyDown(int key);
	void OnKeyUp(int key);

	void ResetHorizontalVelocity();
	void ResetVerticalVelocity();

	void SetElementType(ElementType type);

private:
	int horizontalVelocity;
	int verticalVelocity;
};