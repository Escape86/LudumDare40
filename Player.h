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

private:
	int horizontalVelocity;
	int verticalVelocity;
};