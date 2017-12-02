#pragma once

#include "Object.h"

class Enemy : public Object
{
public:
	Enemy(int x, int y, int targetX, int targetY);
	~Enemy();

	void InjectFrame() override;

private:
	int targetX;
	int targetY;
};