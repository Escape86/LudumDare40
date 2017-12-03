#pragma once

#include "Object.h"

class Enemy : public Object
{
public:
	Enemy(double x, double y, int targetX, int targetY, ElementType type);
	~Enemy();

	void InjectFrame() override;

private:
	int targetX;
	int targetY;
};